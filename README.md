# SecretCapture

1) In the actual code add the following:

uint8 * sc=context-> secret;

 int i=0;

 int a=1;


  while(a==1)
	
 {
		
		for(; i< 32; i++)
  	
  	{
  		
  		printf("%u",sc[i]);
  	
  	}
	i=0;
    printf("\n");
}

This will output the secret to you.

2) Compile it and run it

3) Open a terminal and check for the process id for the run.

4) Using the process id run the following:
		gcore -o <output file name> <process id>

5) Take the output file generated and check for the secret using a hex viewer.
	For Mac, iHex can be used