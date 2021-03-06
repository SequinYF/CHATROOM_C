/*******************************************************************************
 * Author :          Sequin_YF
 * Email :           catherine199787@outlook.com
 * Last modified :   2016-08-05 08:49
 * Filename :        common.c
 * Description :
 * *****************************************************************************/
#include"common.h"


char l_getc(){
    char ch;
    scanf("%c",&ch);
    while(ch == '\n'){
        scanf("%c",&ch);
    }
    return ch;
}

//Linux没有getch()

int getch(void){
    int c= 0;
    struct termios org_opts, new_opts;
    int res = 0;

    //将之前的设置备份
    res = tcgetattr(STDIN_FILENO, &org_opts);
    assert(res == 0);

    //设置新的设定
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON |ECHO | ECHOE | ECHOK |ECHONL | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c= getchar();

    //恢复之前的设定
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res == 0);
    return c;
}

//密码输入

void getpwd(int maxlen, char *pwd){
    int j= 0;
    char c;

    while((j < maxlen-2) && (c = getch()) != '\n'){
        if(c != 127){
            printf("*");
            pwd[j++] = c;
        }
        else {
            if(j > 0){
                 j = j - 1;
                 printf("\b \b");
            }
        }
    }
    pwd[j++] = '\0';
    pwd[j++] = '\n';

    puts(" ");
}

void cl_stdin(){
    char ch;
    while(( ch = getchar()) != '\n' && ch != EOF);
}

//合法输入
int input_check(int input_len, char * input_buf){
    char c;
    int i = 0;
    if(input_buf == NULL){
        my_err("input check", __LINE__);
    }

    while((c = getchar()) != '\n' && (c != EOF) && (i < input_len-2)){
        input_buf[i++] = c;
    }

    input_buf[i++] = '\0';
    input_buf[i++] = '\n';

    return i;
}



//自定义错误函数
void my_err(const char* err_string, int line){
    fprintf(stderr, "line:%d \n", line);
    perror(err_string);
}

//从套接字上读取一次数据

int recv_data(int conn_fd, send_t *precv){
    int len_remain = 0;

        len_remain = recv(conn_fd, precv, sizeof(send_t), 0);
        if(len_remain < 0){
            my_err("recv", __LINE__);
        }

    printf("%d recv\n", precv->command_type);
    return len_remain;
}


//文件写入函数

int file_write(int fd, const void * buf, int len){
    int we;

    we = write(fd , buf, len);
    if(we < 0){
        my_err("file_write", __LINE__);
    }

    return we;
}

//获取文件长度
int file_len(int fd){

    int length = 0;
    if(lseek(fd ,0 ,SEEK_END)== -1){
        my_err("lseek set", __LINE__);
    }
    if((length = lseek(fd , 0 , SEEK_CUR)) == -1 ){
        my_err("length" , __LINE__);
    }
    if(lseek(fd , 0 ,SEEK_SET)== -1){
        my_err("lseek end" , __LINE__);
    }
    return length;
}


int file_read(int fd,int len,void* buf){
    int re;
    if((re = read(fd , buf , len)) == -1){
        my_err("c_read", __LINE__);
    }
    return re;
}

//显示接收信息
/*
void show_recv(int conn_fd, char *recv_buf, int buflen ){
    int i;
    int ret;

    ret = my_recv(conn_fd, recv_buf, buflen);
    if(ret < 0){
        my_err("recv faild", __LINE__);
    }

    for(i = 0; i < ret; i++){
        printf("%c", recv_buf[i]);
    }
}
*/
//获取系统当前日期
user_date_t DateNow() {
	user_date_t curDate;
	time_t now;         //实例化time_t结构
	struct tm *timeNow;         //实例化tm结构指针
	time(&now);
	timeNow = localtime(&now);
	curDate.year=timeNow->tm_year+1900;
	curDate.month=timeNow->tm_mon+1;
	curDate.day=timeNow->tm_mday;

	return curDate;
}

//获取系统当前时间
user_time_t TimeNow(){
	user_time_t curTime;
	time_t now;         //实例化time_t结构
	struct tm *timeNow;         //实例化tm结构指针
	time(&now);
	timeNow = localtime(&now);
	curTime.hour=timeNow->tm_hour;
	curTime.minute=timeNow->tm_min;
	curTime.second=timeNow->tm_sec;

    return curTime;
}

int ser_atoi(char * buf){     //atoi

    int ret;
    ret = atoi(buf);

    return ret;
}
/*
int recv_data(int conn_fd, send_t * psend){
    int n = 0;
    send_t recv_buf;
    char *precv = (char*) &recv_buf;

    if(n == 0){
        n = recv(conn_fd, &recv_buf, sizeof(recv_buf), 0);
        if(n <= 0){
            my_err("recv_", __LINE__);
        }
        else if(n == 0){
            return 0;
        }
        precv = (char*)&recv_buf;
    }

    memcpy(psend, precv, sizeof(send_t));

    return sizeof(send_t);
}
*/

//发送数据
int send_data(int conn_fd, send_t * psend){

    int ret;
    if((ret = send(conn_fd, psend, sizeof(send_t), 0)) < 0){
        my_err("send",__LINE__);
    }


    printf("%u send\n", psend->command_type);
    memset(psend, 0, sizeof(send_t));
    return ret;
}



//将字符串str就地转换为大写字符串，并返回字符串头指针
char *Str2Upper(char *str) {
	if (NULL == str)
		return NULL;
	else {
		char *p = str;
		while ('\0' != *p) {
			if (*p >= 'a' && *p <= 'z')
				*p -= 32;
			p++;
		}
		return str;
	}
}

//显示消息
/*void show_message(int conn_fd){
    message_t leave;
    message_t * pmess = &leave;

    recv(conn_fd, pmess, sizeof(leave), 0);
    printf("%d-%d-%d %2d:%2d:%2d\n",pmess->date.year,pmess->date.month,pmess->date.day,pmess->time.hour,pmess->time.minute,pmess->time.second);
    printf("<%s>: ", pmess->send_name);
    puts(pmess->message_buf);
    puts(" ");
}*/

/*
void recv_message(int conn_fd, message_t * data){

    if(recv(conn_fd, data, sizeof(message_t), 0) < 0){
        send_data(conn_fd,(char *)("n"));
        return;
    }
    else{
        send_data(conn_fd, (char*)("y"));
    }

}


void send_message(int conn_fd, char * send_name){

    static char send_buf[BUFSIZE-36];
    static char recv_buf[BUFSIZE];
    int flag = INVALID_INPUT;

    message_t info;
    message_t *data = &info;

    data->date = DateNow();
    data->time = TimeNow();
    strcpy(data->send_name, send_name);

    input_check(BUFSIZE-36, send_buf);
    strcpy(data->message_buf, send_buf);
     do{

        if(send(conn_fd, data, sizeof(info), 0) < 0){
            my_err("send", __LINE__);
        }

        if(my_recv(conn_fd, recv_buf,sizeof(recv_buf)) < 0){
            my_err("data is too long", __LINE__);
        }

        if(recv_buf[0] == VALID_INPUT){
             flag = VALID_INPUT;
            }
        else{
            printf("error, input again,\n");
             flag = INVALID_INPUT;
            input_check(BUFSIZE-36, send_buf);
            strcpy(data->message_buf, send_buf);
           }
       }while(flag == INVALID_INPUT);

}
*/
