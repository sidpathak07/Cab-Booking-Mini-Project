#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
int main()
{
	double ulat,ulong,otp,udlat,udlong,time1,money;
	int server_response,op,op1,choice,count=0,op2,op3;
	char user_name[30],password[30],clat[30],clong[30],lat[30],lon[30];
	char tim[50],flat[30],flong[30],day[8],month[10],date[2],year[4],msg1[10],udlat1[30],udlong1[30];
	time_t t=time(NULL);
	//creating client socket
	int client_socket;
	FILE *fp;
	client_socket=socket(AF_INET,SOCK_STREAM,0);
	
	//filling info
	struct sockaddr_in client_address;
	client_address.sin_family=AF_INET;
	client_address.sin_port=htons(9002);
	client_address.sin_addr.s_addr=INADDR_ANY;
	
	//connect
	int connection_status=connect(client_socket,(struct sockaddr*)&client_address,sizeof(client_address));
	if(connection_status==-1)
		printf("\nConnection Failed.\n");
	else
		printf("\nConnection Successful.\n");
		
	printf("                    [+]FIND MY CAB[+]                    \n");
	printf("                   1.Create New Account                  \n");
	printf("                   2.Login                               \n");
ch:	scanf("%d",&op);
	send(client_socket,&op,sizeof(op),0);
	switch(op)
	{
		case 1:
			//create new account
			re1:	printf("\nEnter User Name:");
				scanf("%s",&user_name);
				send(client_socket,&user_name,sizeof(user_name),0);
				recv(client_socket,&server_response,sizeof(server_response),0);
				if(server_response==1)
				{
					printf("\nUser Name Valid.\nEnter Password:\t");
					scanf("%s",&password);
					send(client_socket,&password,sizeof(password),0);
					goto main;
				}
				else
				{
					printf("\nUser name exist pls enter another User name.");
					goto re1;
				}

			break;
			
		case 2:
			//login
			re:	printf("\nEnter Username:\t");
				scanf("%s",&user_name);
				printf("\nEnter Password:\t");
				scanf("%s",&password);
				send(client_socket,&user_name,sizeof(user_name),0);
				send(client_socket,&password,sizeof(password),0);
				recv(client_socket,&server_response,sizeof(server_response),0);
				if(server_response==1){
					printf("Login Successful");
					goto main;
				}
				else{
					printf("User name or Password is incorrect please reenter user name and password");
					goto re;
				}
				break;
			
		default:
				printf("\nEnter correct choice.");
				goto ch;
	}
main:	do
	{
		printf("\n                    [+]HOME[+]                           \n");
		if(count==0)
			printf("                   1.Book Cab                            \n");
		printf("                   2.Check Travel History                \n");
		if(count==1)
			printf("                   3.Cancel Cab Booking                  \n");
		printf("                   4.Exit                                \t");
		scanf("%d",&op1);
		if(op1!=2)
			send(client_socket,&op1,sizeof(op1),0);
		switch(op1)
		{
			case 1:
				printf("\nEnter User Latitude:");
				scanf("%lf",&ulat);
				send(client_socket,&ulat,sizeof(ulat),0);
				printf("\nEnter User Longitude:");
				scanf("%lf",&ulong);
				send(client_socket,&ulong,sizeof(ulong),0);
				printf("\nEnter Destination Latitude:");
				scanf("%lf",&udlat);
				send(client_socket,&udlat,sizeof(udlat),0);
				printf("\nEnter Destination Longitude:");
				scanf("%lf",&udlong);
				send(client_socket,&udlong,sizeof(udlong),0);
		
					recv(client_socket,&server_response,sizeof(server_response),0);
					if(server_response==1)
					{
						printf("\nCab Request Accepted.");
						recv(client_socket,&otp,sizeof(otp),0);
						printf("\nOTP:%lf\n",otp);
						recv(client_socket,&time1,sizeof(time1),0);
						recv(client_socket,&money,sizeof(money),0);
						printf("\nApprox Arrival Time(in minitues): %lf",time1);
						printf("\nApprox Travel Charges(in Rs): %lf",money);
						printf("\nConfirm  Cab?\n1.Yes\n2.No.\t");
						scanf("%d",&op2);
						send(client_socket,&op2,sizeof(op2),0);
						if(op2==1)
							count++;
						
					}
					else
						printf("\nCab Requested Rejected.");
					//getchar();
					goto main;
					break;
			case 2:
				if(fp=fopen(user_name,"r")){
				while((fscanf(fp,"%s %s %s %s %s",day,month,date,tim,year)!=EOF)&&(fscanf(fp,"%s %s %s %s %s",lat,lon,udlat1,udlong1,msg1)!=EOF))
				{
					printf("%s %s %s %s %s User Latitude: %s User Longitude: %s Destination Latitude: %s  Destination Longitude: %s Request: %s",day,month,date,tim,year,lat,lon,udlat1,udlong1,msg1);
				};
				fclose(fp);
				}
				else
					printf("\nNo History Found.\n");
				goto main;
				break;
			case 3:
				printf("\nDo You Want to Cancel Cab Booking.\n1.Yes.\n2.No.\t");
				scanf("%d",&op3);
				if(op3==1)
				{
					count--;
					send(client_socket,&op3,sizeof(op3),0);
				}
				break;
			case 4:
				exit(0);
			default:
				printf("\nEnter Correct Choice.");
		}
	}while(1);
	close(client_socket);

}
