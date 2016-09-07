  

  signal( SIGINT , ctrlc_handler ); // set signal for catching ctrl-c
  
  /* For ever loop until ctrl-c key */
  while(quitReq==0)
  {
  	
  	// print menu
		printf("\nChoose an option: enter (letter) then [its argument(s)], then push ENTER:\n\n");
		
		printf("  (a)mbiant ir sensors\n");
		printf("  (ag) accel and gyro sensors\n");
		printf("  (b)attery status\n");
		printf("  (br)aitenberg obstacle avoidance mode\n");
		printf("  (c)amera demo\n");
		printf("  (d)rive the robot with keyboard\n");
		printf("  (i)2c test with KoreMotorLE board\n");
		printf("  (g)et motors speed and position\n");
		printf("  (gp) test pwm and gpios\n");
		printf("  (l)eds [ lr lg lb rr rg rb br bg bb] (max 63)\n");
		printf("  (li)ne following mode\n");
		printf("  (ms) motor speed [left] [right] speed in [pulse/%dms]  (1 pulse/%dms = %.3f mm/s)\n",KH4_TIME_BTWN,KH4_TIME_BTWN,KH4_SPEED_TO_MM_S);
		printf("  (msp) motor speed profile [left] [right] speed in [pulse/%dms]  (1 pulse/%dms = %.3f mm/s)\n",KH4_TIME_BTWN,KH4_TIME_BTWN,KH4_SPEED_TO_MM_S);
		printf("  (mso) motor speed open loop [left] [right] speed ( %d = 100 %%)\n",KH4_MAX_OPENLOOP_SPEED_PWM_100);
		printf("  (mp) motor position [left] [right] absolute position in [pulses]  (1 pulse = %.6f mm)\n",KH4_PULSE_TO_MM);
		printf("  (p)roximity ir sensors\n");	
		printf("  (pi)d [p i d]  current: %d %d %d\n",kp,ki,kd);
		printf("  (pm) position margin [pm]  current: %d\n",pmarg);
		printf("  (s)top motor\n");
		printf("  (sp) speed profile [Acc_inc  Acc_div Min_speed_acc  Min_speed_dec Max_speed]  current: %d %d %d %d %d\n",accinc,accdiv,minspacc, minspdec,maxsp); 
		printf("  (so)und demo\n");	
		printf("  (st)atus led (red on = 1, green on = 16, both on = 0, both off = 17)\n");		
		printf("  (re)set encoders\n");
		printf("  (rm) reset the microcontroller\n");
		printf("  (q)uit program\n");
		printf("  (u)ltrasonic sensors\n");
		printf("  (ua) ultrasonic sensors activate (1:left, 2:front left, 4:front, 8:front right, 16:right, all:31)\n");

		
		printf("\noption: "); 
		
		// wait and save choice
		fgets(line,80,stdin);
		
		
		// applay selected choice
		switch(line[0])
		{
			case 'a': // accel and gyro or ambiant ir sensors
				if ((strlen(line)>1 && (line[1]=='g')))
				{		
					while(!kb_kbhit())
					{	
						kb_clrscr();
						// get gyro sensor
						printf("\ngyro sensor [deg/s]\n       new data                                                      old data    average\ngyro X: ");
						kh4_measure_gyro(Buffer, dsPic);
						dmean=0;
						for (i=0;i<10;i++)
						{
							dval=((short)((Buffer[i*2] | Buffer[i*2+1]<<8)))*KH4_GYRO_DEG_S;
							printf("%6.1f ",dval);
							dmean+=dval;                                                       
						}   
						
						printf(" %6.1f",dmean/10.0);

						printf("\ngyro Y: ");
						dmean=0;
						for (i=10;i<20;i++)
						{

							dval=((short)((Buffer[i*2] | Buffer[i*2+1]<<8)))*KH4_GYRO_DEG_S;
							printf("%6.1f ",dval);
							dmean+=dval;                               	                                                                   
						} 
						printf(" %6.1f",dmean/10.0);
							
						printf("\ngyro Z: ");
						dmean=0;
						for (i=20;i<30;i++)
						{
							dval=((short)((Buffer[i*2] | Buffer[i*2+1]<<8)))*KH4_GYRO_DEG_S;
							printf("%6.1f ",dval);
							dmean+=dval;                               	                                                                   
						}
						printf(" %6.1f",dmean/10.0);     
						printf("\n");



						// get accel sensor
						kh4_measure_acc((char *)Buffer, dsPic);

						printf("\nAcceleration sensor [g]\n       new data                                            old data  average  [g]: -2     -1      0      1      2\nacc  X: ");
						dmean=0;
						for (i=0;i<10;i++)
						{
							dval=((short)(Buffer[i*2] | Buffer[i*2+1]<<8)>>4)/1000.0;
							printf("%5.2f ",dval);
							dmean+=dval;                                                                                       
						}
						
						dval=dmean/10.0;
						
						printf(" %5.2f",dval);   
						
						// compute bar index
						n = (int)abs(dval*ACGY_BAR_LEN/MAX_G /2.0); 
						// fill up bar
						if (dval < 0)
						{
							for (i=0;i<ACGY_BAR_LEN/2-n;i++)
								bar[0][i]=' ';
						 	for (i=ACGY_BAR_LEN/2-n;i<ACGY_BAR_LEN/2;i++)
								bar[0][i]='-';
							bar[0][ACGY_BAR_LEN/2]='0';								
							for (i=ACGY_BAR_LEN/2+1;i<ACGY_BAR_LEN;i++)
								bar[0][i]=' ';  
						}
						else
						{
							for (i=0;i<ACGY_BAR_LEN/2;i++)
								bar[0][i]=' '; 
								
					    bar[0][ACGY_BAR_LEN/2]='0';	
					
							for (i=ACGY_BAR_LEN/2+1;i<ACGY_BAR_LEN/2+n+1;i++)
							  bar[0][i]='+';
				 
							for (i=ACGY_BAR_LEN/2+n+1;i<ACGY_BAR_LEN;i++)
								bar[0][i]=' ';
						}
								 
				 	  bar[0][ACGY_BAR_LEN/2]='0';
					
						bar[0][ACGY_BAR_LEN]='\0';
					
						printf("        |%s|",bar[0]);
							
						printf("\nacc  Y: ");
						dmean=0;
						for (i=10;i<20;i++)
						{
							dval=((short)(Buffer[i*2] | Buffer[i*2+1]<<8)>>4)/1000.0;
							printf("%5.2f ",dval);
							dmean+=dval;
						}
						
						dval=dmean/10.0;
						printf(" %5.2f",dval);
						
												// compute bar index
						n = (int)abs(dval*ACGY_BAR_LEN/MAX_G /2.0); 
						// fill up bar
						if (dval < 0)
						{
							for (i=0;i<ACGY_BAR_LEN/2-n;i++)
								bar[0][i]=' ';
						 	for (i=ACGY_BAR_LEN/2-n;i<ACGY_BAR_LEN/2;i++)
								bar[0][i]='-';
							bar[0][ACGY_BAR_LEN/2]='0';
								
							for (i=ACGY_BAR_LEN/2+1;i<ACGY_BAR_LEN;i++)
								bar[0][i]=' ';  
						}
						else

						{
							for (i=0;i<ACGY_BAR_LEN/2;i++)
								bar[0][i]=' '; 
								

					    bar[0][ACGY_BAR_LEN/2]='0';	
					
							for (i=ACGY_BAR_LEN/2+1;i<ACGY_BAR_LEN/2+n+1;i++)
							  bar[0][i]='+';
				 
							for (i=ACGY_BAR_LEN/2+n+1;i<ACGY_BAR_LEN;i++)

								bar[0][i]=' ';
						}
								 
				 	  bar[0][ACGY_BAR_LEN/2]='0';
					
						bar[0][ACGY_BAR_LEN]='\0';
					
						printf("        |%s|",bar[0]);
						
						printf("\nacc  Z: ");
						dmean=0;
						for (i=20;i<30;i++)
						{
							dval=((short)(Buffer[i*2] | Buffer[i*2+1]<<8)>>4)/1000.0;
							printf("%5.2f ",dval);
							dmean+=dval;
						}
						
						dval=dmean/10.0;
						printf(" %5.2f",dval);
						
						// compute bar index
						n = (int)abs(dval*ACGY_BAR_LEN/MAX_G/2.0); 
						// fill up bar
						if (dval < 0)
						{
							for (i=0;i<ACGY_BAR_LEN/2-n;i++)
								bar[0][i]=' ';
						 	for (i=ACGY_BAR_LEN/2-n;i<ACGY_BAR_LEN/2;i++)
								bar[0][i]='-';
							bar[0][ACGY_BAR_LEN/2]='0';
								
							for (i=ACGY_BAR_LEN/2+1;i<ACGY_BAR_LEN;i++)
								bar[0][i]=' ';  
						}
						else
						{
							for (i=0;i<ACGY_BAR_LEN/2;i++)
								bar[0][i]=' '; 
								
					    bar[0][ACGY_BAR_LEN/2]='0';	
					
							for (i=ACGY_BAR_LEN/2+1;i<ACGY_BAR_LEN/2+n+1;i++)
							  bar[0][i]='+';
				 
							for (i=ACGY_BAR_LEN/2+n+1;i<ACGY_BAR_LEN;i++)
								bar[0][i]=' ';
						}
								 
				 	  bar[0][ACGY_BAR_LEN/2]='0';
					
						bar[0][ACGY_BAR_LEN]='\0';
					
						printf("        |%s|",bar[0]);
						
						
						printf("\n\nPush any key to stop\n");
						usleep(200000); // wait 200ms
					}	
				}
				else
				{
					while(!kb_kbhit())
					{	
						kb_clrscr();	
						// get ir sensor
						kh4_ambiant_ir(Buffer, dsPic);
				
						for (i=0;i<12;i++)
						{
							sensors[i]=(Buffer[i*2] | Buffer[i*2+1]<<8);
											                       
							n=(int)(sensors[i]*IR_BAR_LEN/1024.0);
											                      
							if (n==0)
								sprintf(bar[i],"|\33[%dC>|",IR_BAR_LEN-1);
							else
								if (n>=IR_BAR_LEN-1)
									sprintf(bar[i],"|>\33[%dC|",IR_BAR_LEN-1);
								else
								 sprintf(bar[i],"|\33[%dC>\33[%dC|",IR_BAR_LEN-1-n,n);
											                      
						 }                                
						 printf("\n                   bright                dark\
						 \nback left      : %4.4u  %s\nleft           : %4.4u  %s\
						 \nfront left     : %4.4u  %s\nfront          : %4.4u  %s\
						 \nfront right    : %4.4u  %s\nright          : %4.4u  %s\
						 \nback right     : %4.4u  %s\nback           : %4.4u  %s\
						 \nground left    : %4.4u  %s\ngnd front left : %4.4u  %s\
						 \ngnd front right: %4.4u  %s\nground right   : %4.4u  %s\n",
							 sensors[0],bar[0],  sensors[1],bar[1],
							 sensors[2],bar[2],  sensors[3],bar[3],
							 sensors[4],bar[4],  sensors[5],bar[5],
							 sensors[6],bar[6],  sensors[7],bar[7],
							 sensors[8], bar[8], sensors[9], bar[9],
							 sensors[10], bar[10],sensors[11], bar[11]
							 );
		
						printf("\nPush any key to stop\n");
						usleep(200000); // wait 200ms
					}
				}
				tcflush(0, TCIFLUSH);
				
			break;
			case 'b': // braitenberg or battery status
				if ((strlen(line)>2 && (line[1]=='r'))) {
					// braitenberg
					braitenbergAvoidance();
				}
				else
				{ // battery status
					while(!kb_kbhit())
					{	
						kb_clrscr();	
						kh4_battery_status(Buffer,dsPic);
						printf("Battery:\n  status (DS2781)   :  0x%x\n",Buffer[0]);
						printf("  remaining capacity:  %4.0f mAh\n",(Buffer[1] | Buffer[2]<<8)*1.6);
						printf("  remaining capacity:   %3d %%\n",Buffer[3]);
						printf("  current           :  %4.0f mA\n",(short)(Buffer[4] | Buffer[5]<<8)*0.07813);
						printf("  average current   :  %4.0f mA\n",(short)(Buffer[6] | Buffer[7]<<8)*0.07813);
						printf("  temperature       :  %3.1f C \n",(short)(Buffer[8] | Buffer[9]<<8)*0.003906);
						printf("  voltage           :  %4.0f mV \n",(Buffer[10] | Buffer[11]<<8)*9.76);
						printf("  charger           :  %s\n",kh4_battery_charge(dsPic)?"plugged":"unplugged");
						printf("\nPush any key to stop\n");
						usleep(200000); // wait 200ms
					}
				}
				tcflush(0, TCIFLUSH);
			break;			
			case 'c': // camera
				camera_example();
			break;
		
			case 'd': // drive mode
				drive_robot();
			break;
		
			case 'g': // test gpio and pwm  or get motor speed and position		
				if ((strlen(line)>2 && (line[1]=='p'))) {	
					test_gpio_and_pwm();
				}
				else
				{ // get motor speed and position		
					while(!kb_kbhit())
					{	
						kb_clrscr();		
						kh4_get_speed(&sl,&sr,dsPic);
						kh4_get_position(&pl,&pr,dsPic);
						printf("motors speed [mm/s (pulse/)]: left: %7.1f  (%5d)  | right: %7.1f (%5d)\n",sl*KH4_SPEED_TO_MM_S,sl,sr*KH4_SPEED_TO_MM_S,sr);
						printf("motors position [mm (pulse)]: left: %7.1f (%7d) | right: %7.1f (%7d)\n",pl*KH4_PULSE_TO_MM,pl,pr*KH4_PULSE_TO_MM,pr);
					
						printf("\nPush any key to stop\n");
						usleep(200000); // wait 200ms
					
					}
					tcflush(0, TCIFLUSH); // flush input	
				}
			break;
			
			
			case 'i': // i2c tests
				i2c_test();
			break;
			
			case 'l': // line following or leds
				if ((strlen(line)>2 && (line[1]=='i'))) {
					line_following();
				}
				else {
				// leds
					if (EOF==sscanf(line,"%*c %d %d %d %d %d %d %d %d %d",&l[0],&l[1],&l[2],&l[3],&l[4],&l[5],&l[6],&l[7],&l[8]))
						printf("\n*** ERROR ***: the led commands must be 9 integers\n");
					else	
					{	
						kh4_SetRGBLeds(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8], dsPic);
						printf("rgb %d %d %d %d %d %d %d %d %d\n",l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8]);
					}	
				}
			break;		
			case 'm': // set motors speed profile or speed or position
				if ((strlen(line)>2 && (line[1]=='s') && (line[2]=='p')))
				{ // speed profile
					sl=sr=0;
					if (EOF==sscanf(line,"%*c %*c %*c %d %d",&sl,&sr))
						printf("\n*** ERROR ***: the motors speeds must be integers\n");
					else
						printf("\nspeeds to set: %d %d with speed profile\n",sl,sr);	
						kh4_SetMode(kh4RegSpeedProfile,dsPic );
						kh4_set_speed(sl,sr, dsPic);
				}	else
				if ((strlen(line)>2 && (line[1]=='s') && (line[2]=='o')))
				{ // speed openloop
					sl=sr=0;
					if (EOF==sscanf(line,"%*c %*c %*c %d %d",&sl,&sr))
						printf("\n*** ERROR ***: the motors speeds must be integers\n");
					else
						printf("\nspeeds to set: %d %d with openloop\n",sl,sr);	
						kh4_SetMode(kh4RegSOpenLoop,dsPic );
						kh4_set_speed(sl,sr, dsPic);
				}	else
				if ((strlen(line)>2 && (line[1]=='s')))
				{ // speed
					sl=sr=0;
					if (EOF==sscanf(line,"%*c %*c %d %d",&sl,&sr))
						printf("\n*** ERROR ***: the motors speeds must be integers\n");
					else
						printf("\nspeeds to set: %d %d with speed\n",sl,sr);	
						kh4_SetMode(kh4RegSpeed,dsPic );
						kh4_set_speed(sl,sr, dsPic);
				}	else
				if ((strlen(line)>1 && (line[1]=='p')))
				{ // position
					sl=sr=0;
					if (EOF==sscanf(line,"%*c %*c %d %d",&sl,&sr))
						printf("\n*** ERROR ***: the motors positions must be integers\n");
					else
						printf("\nposition to set: %d %d with position regulation\n",sl,sr);	
						kh4_SetMode(kh4RegPosition,dsPic );
						kh4_set_position(sl,sr, dsPic);
				}
			break;
			case 'p':  //pid,position margin, proximity sensors
				if ((strlen(line)>2 && (line[1]=='i')))
				{ // pid
					if (EOF!=sscanf(line,"%*c%*c %d %d %d",&kp,&ki,&kd))
					{
						kh4_ConfigurePID(kp,ki,kd,dsPic);
					}
				}
				else
				if ((strlen(line)>2 && (line[1]=='m')))
				{ // position margin
					if (EOF!=sscanf(line,"%*c%*c %d",&pmarg))
					{
						kh4_SetPositionMargin(pmarg,dsPic);
					}
				}
				else
				{ // proximity sensors
					while(!kb_kbhit())
					{	
						kb_clrscr();		
		
						// get ir sensor
						kh4_proximity_ir(Buffer, dsPic);
				
						for (i=0;i<12;i++)
						{
							sensors[i]=(Buffer[i*2] | Buffer[i*2+1]<<8);
											                       
							n=(int)(sensors[i]*IR_BAR_LEN/1024.0);
											                      
							if (n==0)
								sprintf(bar[i],"|\33[%dC>|",IR_BAR_LEN-1);
							else
								if (n>=IR_BAR_LEN-1)
									sprintf(bar[i],"|>\33[%dC|",IR_BAR_LEN-1);
								else
								 sprintf(bar[i],"|\33[%dC>\33[%dC|",IR_BAR_LEN-1-n,n);
											                      
						 }                                
						 printf("\n                    near               far\
						 \nback left      : %4u  %s\nleft           : %4u  %s\
						 \nfront left     : %4u  %s\nfront          : %4u  %s\
						 \nfront right    : %4u  %s\nright          : %4u  %s\
						 \nback right     : %4u  %s\nback           : %4u  %s\
						 \nground left    : %4u  %s\ngnd front left : %4u  %s\
						 \ngnd front right: %4u  %s\nground right   : %4u  %s\n",
							 sensors[0],bar[0],  sensors[1],bar[1],
							 sensors[2],bar[2],  sensors[3],bar[3],
							 sensors[4],bar[4],  sensors[5],bar[5],
							 sensors[6],bar[6],  sensors[7],bar[7],
							 sensors[8], bar[8], sensors[9], bar[9],
							 sensors[10], bar[10],sensors[11], bar[11]
							 );
		
						printf("\nPush any key to stop\n");
						usleep(200000); // wait 200ms

					}
				}
				tcflush(0, TCIFLUSH); // flush input	
			break;	
			case 'q': // quit
				quitReq=1;
				break;
			case 'r': // reset  encoders the microcontroller
				if ((strlen(line)>1 && (line[1]=='e')))
				{ //or encoders
					kh4_ResetEncoders(dsPic);
				}
				else
					if ((strlen(line)>1 && (line[1]=='m')))
					{// reset the microcontroller 
						kh4_SetStatusLeds(KH4_ST_LED_RED_ON, dsPic);
						usleep(10000); // wait 10ms
						kh4_reset(dsPic);
						printf("\nWait for microcontroller to reset...\n");
						sleep(3); // wait for Koala to restart
					}	
			break;
			case 's': // status leds, speed profile or stop motor 
				if ((strlen(line)>2 && (line[1]=='t')))
				{
					if (EOF!=sscanf(line,"%*c%*c %d",&n))
					{
						// status leds
						kh4_SetStatusLeds(n,dsPic);				
					}
				}
				else					
				if ((strlen(line)>2 && (line[1]=='p')))
				{ // speed profile
					if (EOF!=sscanf(line,"%*c%*c %d %d %d %d %d",&accinc,&accdiv,&minspacc, &minspdec,&maxsp))
					{
						kh4_SetSpeedProfile(accinc,accdiv,minspacc, minspdec,maxsp,dsPic );	
					}
				}
				else
				if ((strlen(line)>2 && (line[1]=='o')))
				{
					test_sound();
				}
				else
				{
					//stop motor
					kh4_set_speed(0 ,0 ,dsPic); // stop robot
					kh4_SetMode( kh4RegIdle,dsPic ); // set motors to idle
				}		
			break;
			case 'u': // us sensors
				if ((strlen(line)>2 && (line[1]=='a')))
				{ // activate us
					if (EOF!=sscanf(line,"%*c%*c %d",&c))
					{
						kh4_activate_us(c,dsPic);				
					}
				}
				else
				{ // display us
					while(!kb_kbhit())
					{	
						kb_clrscr();
		
						// get and print us sensors
						kh4_measure_us(Buffer,dsPic);
				 		for (i=0;i<5;i++)
				 		{
				 			usvalues[i] = (short)(Buffer[i*2] | Buffer[i*2+1]<<8);   
				 			
							if((usvalues[i] == KH4_US_DISABLED_SENSOR) || (usvalues[i] == KH4_US_NO_OBJECT_IN_RANGE))
				 			{ // out of range or disabled
								sprintf(bar[i],"|\33[%dC|",US_BAR_LEN);	
							}  else
							{
								// in range or less than 25cm
								n=(int)(usvalues[i]*US_BAR_LEN/MAX_US_DISTANCE);
																				        
								if (n==0)
									sprintf(bar[i],"|>\33[%dC|",US_BAR_LEN-1);
								else
									if (n>=US_BAR_LEN-1)
										sprintf(bar[i],"|\33[%dC>|",US_BAR_LEN-1);
									else
									 sprintf(bar[i],"|\33[%dC>\33[%dC|",n,US_BAR_LEN-1-n); 
							}                                 
				 		}
				 		
				 		
						printf("\nUS sensors : distance [cm]\
										\n                     50  100  150  200\
										\n                0|    .    :    .    :    |%.0f\nleft 90   : %4d %s  %s\nleft 45   : %4d %s  %s\
						\nfront     : %4d %s  %s\nright 45  : %4d %s  %s\nright 90  : %4d %s  %s\n",MAX_US_DISTANCE,
							 usvalues[0],bar[0],US_VAL(usvalues[0]),usvalues[1],bar[1],US_VAL(usvalues[1]),usvalues[2],bar[2],US_VAL(usvalues[2]),usvalues[3],bar[3],US_VAL(usvalues[3]),usvalues[4],bar[4],US_VAL(usvalues[4])); 
						printf("\nPush any key to end program\n");
						usleep(200000); // wait 200ms
					}
					tcflush(0, TCIFLUSH); // flush input
				}	
			break;
			default: printf("\n*** ERROR ***: option '%c' (0x%02x) is undefined!\n\n",line[0],line[0]);		
		}

  } // while
  
	
  kh4_set_speed(0 ,0 ,dsPic); // stop robot
  kh4_SetMode( kh4RegIdle,dsPic ); // set motors to idle
  kh4_SetRGBLeds(0,0,0,0,0,0,0,0,0,dsPic); // clear rgb leds because consumes energy
