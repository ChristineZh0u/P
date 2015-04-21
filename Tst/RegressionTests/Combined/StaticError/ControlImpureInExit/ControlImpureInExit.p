// P semantics test, one machine: "Control Impure" static errors
// Cases covered:
// "push", "pop" and "raise" in "exit" function,
// in both anon and named functions

event E1 assert 1;
event E2 assert 1;
event E3 assert 1;
event unit assert 1;

main machine Real1 {
    var i: int;	
    start state Real1_Init {
        entry { 
			raise unit;
			 }
			 
		on unit do { send this, E1; 
		             send this, E2; 
		             send this, E3; 
					 push Real1_S1; };   
		on E2 do Action1;   
        on E1 do { push Real1_S1;}; 	
		
        exit { 
			push Real1_S2;                  //error
			pop;                            //no error - OK, only first one is reported
			raise unit;                     //no error - OK
			if (i == 3) {
				    pop;                   //no error
			}
            else
			    {
					i = i + Action4() +   //error
							Action5() -   //error 
							Action6();    //error
			    }
			}
	}
	state Real1_S1 {
		entry {}
		ignore E1;
	    defer E2;    
		on E3 do { pop; };
		exit Action1;                        //error
    }
	state Real1_S2 {
		entry { }
		exit { pop; }                        //error
	}
	state Real1_S3 {
		entry { }
		exit Action2;                        //error
	}
	state Real1_S4 {
		entry { }
		exit { raise unit; }                 //error
	}
	state Real1_S5 {
		entry { }
		exit Action3;                        //error
	}
	fun Action1() {		                          
		pop;                                 
    }
	fun Action2() {
		push Real1_S1;
    }
	fun Action3() {
		raise unit;
    }
	fun Action4() : int {		                          
		pop;   
		return 1;
    }
	fun Action5() : int {
		push Real1_S1;
		return 1;
    }
	fun Action6() : int {
		raise unit;
		return 1;
    }
}