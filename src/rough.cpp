	string hellohii = to_string(player1.getPosX()) + "+" + to_string(player1.getPosX());
							
							helloserv = new char[hellohii.length()];
							for (int i = 0; i < hellohii.length(); i++)
							{
								/* code */
								helloserv[i] = hellohii.at(i);
								
							}
							string positionX = "";
							string positionY = "";
							
							 if(send(new_socketserv, helloserv, strlen(helloserv), 0)>=0){}
							  valreadserv = read(new_socketserv, bufferserv, 1024);
                            //  printf("%s\n", bufferserv);
							int pos;
							
							for(int i = 0; i < 7; i++){
								if(intchker(bufferserv[i]) == 1){pos = i;break;}
								if(intchker(bufferserv[i]) == 2){
									positionX.push_back(bufferserv[i]);
								}

							}
							for(int i = pos+1; i < 10; i++){
								if(intchker(bufferserv[i]) == 1){pos = i;break;}
								if(intchker(bufferserv[i]) == 2){
									positionY.push_back(bufferserv[i]);
								}

							}

							
							player2.changePos(std::stoi(positionX),std::stoi(positionY));
							cout << bufferserv << endl;






		string hellohii = to_string(player1.getPosX()) + "+" + to_string(player1.getPosX());
							
							hellocl = new char[hellohii.length()];
							for (int i = 0; i < hellohii.length(); i++)
							{
								/* code */
								hellocl[i] = hellohii.at(i);
								
							}

							 send(sockcl, hellocl, strlen(hellocl), 0);
							 valreadcl = read(sockcl, buffercl, 1024);
                            //  printf("%s\n", buffercl);
							
							string positionX = "";
							string positionY = "";
							
							 if(send(new_socketserv, helloserv, strlen(helloserv), 0)>=0){}
							  valreadserv = read(new_socketserv, bufferserv, 1024);
                            //  printf("%s\n", bufferserv);
							int pos;
							
							for(int i = 0; i < 7; i++){
								if(intchker(bufferserv[i]) == 1){pos = i;break;}
								if(intchker(bufferserv[i]) == 2){
									positionX.push_back(bufferserv[i]);
								}

							}
							for(int i = pos+1; i < 10; i++){
								if(intchker(bufferserv[i]) == 1){pos = i;break;}
								if(intchker(bufferserv[i]) == 2){
									positionY.push_back(bufferserv[i]);
								}

							}

							player1.changePos(std::stoi(positionX),std::stoi(positionY));
							cout << positionX << endl;

