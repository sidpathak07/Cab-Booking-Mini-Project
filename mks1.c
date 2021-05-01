#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<math.h>
#include<time.h>
double pi=3.142;
double earth_radius=6371.0;
int ran(int min,int max)
{
	return min+rand()%(max-min);
}
double degtorad(double deg)
{
	return(deg*pi/180);
}
double distanceCal(double lat1d,double lon1d,double lat2d,double lon2d)
{		
	double lat1,lon1,lat2,lon2,delta_lon,central_angle;
	lat1=degtorad(lat1d);
	lon1=degtorad(lon1d);
	lat2=degtorad(lat2d);
	lon2=degtorad(lon2d);

	delta_lon=lon2-lon1;
	
	central_angle=acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(delta_lon));
	return(earth_radius*central_angle);	
}

int main()
{
	double ulat,ulong,otp,dlat=16.6954097,dlong=74.2246642,udlat,udlong,dist1,dist,time1,money;
	int valid=1,not_valid=0,op,count=0,count1,op1,op2,op3;
	char user_name[30],password[30],uname[30],upass[30],c,pass[30],msg[10],msg1[10];
	int network_socket;
	time_t t=time(NULL);
	FILE *fp;
	//creating server socket
	int server_socket;
	server_socket=socket(AF_INET,SOCK_STREAM,0);

	//recv(network_socket,&test,sizeof(test),0);
	//recv(network_socket,&ulat,sizeof(ulat),0);
	//printf("\n[+]Server Socket Created Successfully.");
	//filling info
	struct sockaddr_in server_address;
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(9002);
	server_address.sin_addr.s_addr=INADDR_ANY;	

	
	//binding
	bind(server_socket,(struct sockaddr *)&server_address,sizeof(server_address));

	//listning
	listen(server_socket,5);

	
	//connecting
	
	network_socket=accept(server_socket,NULL,NULL);

ch:	recv(network_socket,&op,sizeof(op),0);
	if(op==1)
	{
		fp = fopen("userpass1", "r");

	// Extract characters from file and store in character c 
		for (c = getc(fp); c != EOF; c = getc(fp)) 
			if (c == '\n') // Increment count if this character is newline 
				count = count + 1; 
	printf("\nNumber of lines= %d",count);
	// Close the file 
		fclose(fp);
	re1:	recv(network_socket,&user_name,sizeof(uname),0);
		fp=fopen("userpass1","r");
		count1=0;
		while(fscanf(fp,"%s\t%s",uname,upass)!=EOF)
		{
			if(strcmp(user_name,uname)!=0)
			{
				count1=count1+1;
			}
		};
		//count1=count1/2;
		printf("\nCount1=%d",count1);
		fclose(fp);		
		if(count1==count)
		{
			send(network_socket,&valid,sizeof(valid),0);
			//recv(network_socket,&password,sizeof(password),0);
			fp=fopen("userpass1","a");
			recv(network_socket,&password,sizeof(password),0);
			fprintf(fp,"%s\t%s\n",user_name,password);
			fclose(fp);
			goto main;
		}
		else
		{
			send(network_socket,&not_valid,sizeof(not_valid),0);
			goto re1;
		}
	
	}
	if(op==2)
	{
		log:	recv(network_socket,&user_name,sizeof(user_name),0);
			recv(network_socket,&password,sizeof(password),0);
			fp=fopen("userpass1","r");
			while(fscanf(fp,"%s\t%s\n",uname,upass)!=EOF)
			{
				if(strcmp(uname,user_name)==0)
				{
					if(strcmp(upass,password)==0)
					{
						send(network_socket,&valid,sizeof(valid),0);
						goto main;
						break;
					}
				}
			}
			if(fscanf(fp,"%s\t%s\n",uname,upass)==EOF)
			{
				send(network_socket,&not_valid,sizeof(not_valid),0);
				goto log;
			}
			fclose(fp);
	}
	if((op!=1)&&(op!=2))
		goto ch;	
	
main:	while(1){
			recv(network_socket,&op1,sizeof(op1),0);	
			if(op1==1)
			{
				recv(network_socket,&ulat,sizeof(ulat),0);
				//printf("\nUser Lattitude : %lf",ulat);
				recv(network_socket,&ulong,sizeof(ulong),0);
				//printf("\nUser Longitude : %lf",ulong);
				recv(network_socket,&udlat,sizeof(udlat),0);
				recv(network_socket,&udlong,sizeof(udlong),0);
				if((distanceCal(dlat,dlong,ulat,ulong)<5)&&(distanceCal(ulat,ulong,udlat,udlong)<30))
				{
					send(network_socket,&valid,sizeof(valid),0);
					otp=rand()%10000;
					send(network_socket,&otp,sizeof(otp),0);
					dlat=udlat;
					dlong=udlong;
					dist=distanceCal(dlat,dlong,ulat,ulong);
					dist1=distanceCal(ulat,ulong,udlat,udlong);
					time1=dist*3;
					money=dist1*5+10;
					send(network_socket,&time1,sizeof(time1),0);
					send(network_socket,&money,sizeof(money),0);
					recv(network_socket,&op2,sizeof(op2),0);
					if(op2==1)
					{
						//sprintf(msg,"CONFIRMED");
						fp=fopen(user_name,"a");
						sprintf(msg,"CONFIRMED");
						fprintf(fp,"%s %lf %lf %lf %lf %s\n",asctime(localtime(&t)),ulat,ulong,udlat,udlong,msg);
						fclose(fp);
						
					}
					if(op2==2)
					{
						sprintf(msg,"REJECTED");
						fp=fopen(user_name,"a");
						fprintf(fp,"%s %lf %lf %lf %lf %s\n",asctime(localtime(&t)),ulat,ulong,udlat,udlong,msg);
						fclose(fp);
					}				
				}
					
					
				else
				{
					send(network_socket,&not_valid,sizeof(not_valid),0);
				}
				}
			if(op1==3)
			{
				recv(network_socket,&op3,sizeof(op3),0);
				sprintf(msg1,"CANCELLED");
				fp=fopen(user_name,"a");
				fprintf(fp,"%s %lf %lf %lf %lf %s\n",asctime(localtime(&t)),ulat,ulong,udlat,udlong,msg1);
				fclose(fp);	
			}
 			if(op1==4)
				exit(0);
			
}

	close(server_socket);
	//}		
}
