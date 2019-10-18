﻿using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using Plang.Compiler;
using UnitTests.Runners;
using UnitTests.Validators;

namespace UnitTests.Core
{
    /// <summary>
    ///     Factory for creating test cases from structured directories on disk.
    /// </summary>
    public class TestCaseFactory
    {

        private readonly DirectoryInfo testTempBaseDir;

        /// <summary>
        ///     Box a new factory with the given scratch directory
        /// </summary>
        /// <param name="testTempBaseDir">The parent directory for each test's scratch directories</param>
        public TestCaseFactory(DirectoryInfo testTempBaseDir)
        {
            this.testTempBaseDir = testTempBaseDir;
        }

        /// <summary>
        ///     Box a test case from the given directory and parsed Prt run configuration
        /// </summary>
        /// <param name="testDir">The directory containing P source files</param>
        /// <param name="output">The desired output language</param>
        /// <returns>The test case in a runnable state.</returns>
        public CompilerTestCase CreateTestCase(DirectoryInfo testDir, CompilerOutput output)
        {
            var inputFiles = testDir.GetFiles("*.p");
            var testName = new Uri(Constants.TestDirectory + Path.DirectorySeparatorChar)
                .MakeRelativeUri(new Uri(testDir.FullName))
                .ToString();

            ICompilerTestRunner runner;
            ITestResultsValidator validator;
    
            int expectedExitCode;

            if (testName.Contains("/DynamicError/") || testName.Contains("/DynamicErrorPrtSharp/"))
            {
                expectedExitCode = 1;
            }
            else if (testName.Contains("/Correct/"))
            {
                expectedExitCode = 0;
            }
            else {
                throw new CompilerTestException(TestCaseError.UnrecognizedTestCaseType);
            }
            
            if (output.Equals(CompilerOutput.C))
            {
                var nativeFiles = testDir.GetFiles("*.c");
                runner = new PrtRunner(inputFiles, nativeFiles);
            }
            else if (output.Equals(CompilerOutput.PSharp))
            {
                var nativeFiles = testDir.GetFiles("*.cs");
                runner = new PSharpRunner(inputFiles, nativeFiles);
            }
            else
            {
                throw new ArgumentOutOfRangeException();
            }

            validator = new ExecutionOutputValidator(expectedExitCode);

            var tempDirName =
                Directory.CreateDirectory(Path.Combine(testTempBaseDir.FullName, output.ToString(), testName));
            return new CompilerTestCase(tempDirName, runner, validator);
        }

        public CompilerTestCase CreateTestCase(DirectoryInfo testDir)
        {
            var inputFiles = testDir.GetFiles("*.p");
            var testName = new Uri(Constants.TestDirectory + Path.DirectorySeparatorChar)
                .MakeRelativeUri(new Uri(testDir.FullName))
                .ToString();

            ICompilerTestRunner runner;
            ITestResultsValidator validator;

            var output = CompilerOutput.C;
            runner = new CompileOnlyRunner(output, inputFiles);

            // TODO: validate information about the particular kind of compiler error
            var isStaticError = testName.Contains("/StaticError/");
            validator = isStaticError
                ? (ITestResultsValidator) new StaticErrorValidator()
                : new CompileSuccessValidator();

            var tempDirName =
                Directory.CreateDirectory(Path.Combine(testTempBaseDir.FullName, output.ToString(), testName));
            return new CompilerTestCase(tempDirName, runner, validator);
        }
    }
}