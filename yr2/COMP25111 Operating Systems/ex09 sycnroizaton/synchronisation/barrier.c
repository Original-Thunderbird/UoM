
    for (i = 0; i < FAMILY; i++)// Barrier
      {
      new_PID = wait(&status);	// Wait for one child state change
      printf("\n[%d]: Child %d (PID %d) has exited ", 
               getpid(), find_child(new_PID), new_PID);
      if (status == NULL) printf("normally.\n");
      else                printf("badly.\n");
      }		// See "man 2 wait" if you want to diagnose any problems.
