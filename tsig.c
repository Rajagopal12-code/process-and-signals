#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>



#define NUM_CHILD 13
#define WITH_SIGNALS

#ifdef WITH_SIGNALS
	char int_flag = 0;
	
	void IntHandler() {
		printf("parent [%i]:  interrupt received.\n",getpid());
		int_flag = 1;
	}
	
	
	void killedChild() {
		printf("child [%i]: process Terminated.", getpid());
	}
#endif

int main(){
	pid_t child_pid=1;		// child process 
	pid_t parent_pid=1;		// parent process
	int m,n,cnt=0;			
	int a,s;					
	
	printf("parent[%i]: parent process is starting.\n",getpid());
	for (m = 0; m < NUM_CHILD; ++m) {
		parent_pid=getpid();
		
		#ifdef WITH_SIGNALS
			for(n= 0; n < NSIG; ++n){  // NSIG - total number of signals 
			sigaction(n,NULL,NULL);
			}
			
				
			signal (SIGCHLD,SIG_DFL);
			/*
			 symbol of this interrupt signal is SIGINT
			*/
			signal (SIGINT,IntHandler);
		#endif
		if (!(child_pid = fork())){
			/*
			In  child process avoid  keyboard interrupt signal handling.
			*/
			#ifdef WITH_SIGNALS
				signal (SIGINT,killedChild);
				signal (SIGINT,SIG_DFL);
			#endif
			printf("child [%i]: Process created.\n",getpid());
			sleep(10);
			printf("child [%i]:  execution completed.\n",getpid());
			exit(0);
			}
		else if (child_pid == -1){
			printf("parent [%i]: Can't create a new child.\n",getpid());
			kill(-1,SIGTERM);
			exit(1);
		}
		sleep(1);
		
	
		#ifdef WITH_SIGNALS
			if (int_flag){
				printf("parent [%i]:  creating process Interrupt!\n", getpid());
				kill(-2,SIGTERM);
				break;
			}
		#endif
	}
	
	while(1){
		a= wait(&s);
		if(a == -1)
			break;
		else{
			printf("child [%i]: process completed.\n",a);
			cnt++;
		}
	}
	printf("\nSuccess: %d processes terminated.\n", cnt);
	/*
	the service handlers of all signals should be restored at the end of main process.
	*/
	#ifdef WITH_SIGNALS
		for(n=0; n<NSIG;n++)
			signal(n,SIG_DFL);
	#endif
	return 0;
}
	

