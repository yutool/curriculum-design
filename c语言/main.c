#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define H "---------------------------------------------------------"
#define N 10
typedef struct _book{
    char book_name[20];  // 书名
    char author[20];     // 作者
    char isbn[20];       // ISBN号
    char publisher[50];  // 出版单位
    int pub_data;        // 出版年份
    float price;         // 价格
	int stock;		     //库存
    struct _book *next;
    struct _book *prev;
}Book;
int psd_validate(char *, char *);

int psd_landing();

Book* book_form();

void book_insert(Book *);

int book_delete(Book *, Book *);

void book_display(Book *, Book *);

void book_inspect(Book *, Book *);

void book_alter(Book *, Book *);

void book_seek(Book *);

void book_construct(Book *);

void book_read(Book *);

void book_write(Book *);

void sort_of_bookname(Book *);

void book_unloose(Book *);

void default_book();

int show_num(Book *);

void show_std();

void show_admin(Book *);

void show_admin_meneu();

void show_std_meneu();

void show_books(Book *);

void show_insert();

void show_seek();

void show_book(Book *);

void show_landing();

void show_manage();

void pack_admin_title(Book *, Book *);

void pack_std_title(Book*, Book*);

void pack_up(Book **, Book *);

void pack_begin(Book **, Book *);

void pack_down(Book **, Book *);

void pack_insert(Book *);

void pack_delete(Book *, Book *);

void pack_seek(Book *);

void pack_admin_landing();

void pack_std_landing();

void show_login();

int obtain_account(char account[N][20]);

void login();

Book* dingwei(Book *, Book *);

void add_stock(Book *temp, Book *book);

void reduce_stock(Book *temp, Book *book);

void borrow_book(Book *temp, Book *book);

void admin();

void denglu();

void student();

int main()
{
	denglu();
	//admin();
	//student();
	return 0;
}

//----------------------- 系统主菜单 -----------------------
void denglu(){
    int chioce;
	system("cls");
    show_landing();
    scanf("%d", &chioce);
    fflush(stdin);
    switch(chioce){
        case 1:		 // 借阅登录
            pack_std_landing();
            break;
        case 2:		// 管理员登录
            pack_admin_landing();
		case 3:		// 学生注册
			login();
			break;
        case 4:		// 修改密码
            printf("\t该功能未实现,");
            system("pause");
            denglu();
            break;
        case 5:		// 退出
            exit(0);
        default:	// 其他
            printf("\t输入的数字有误，");
            system("pause");
            denglu();
            break;
    }
}

//----------------------- 学生借阅系统 -----------------------
void student(){
    Book *book = book_form();
    Book *temp = book;
    char chioce;
    book_read(book);
    pack_std_title(temp, book);

    while(1){
        printf("\t请选择功能：");
        chioce = getch();
        fflush(stdin);
        switch(chioce){
            case 'a': // 查找图书
                pack_seek(book);
                break;
            case 'b': // 借书
                borrow_book(temp, book);
                book_write(book);
                break;
            case 'c': // 查看图书信息
                book_inspect(temp, book);
                break;
            case 'd': // 已借图书
                //待实现
                break;
            case 'e': // 还书
                //待实现
                break;
            case 'f': // 退出
                book_unloose(book);
                exit(0);
            case 72:  //返回首页
                pack_begin(&temp, book);
                break;
            case 75: // 上一页
                pack_up(&temp, book);
                break;
            case 77: // 下一页
                pack_down(&temp, book);
                break;
            default:
                break;
        }
        pack_std_title(temp, book);
    }
    book_unloose(book);
}

//----------------------- 管理员管理系统 -----------------------
void admin(){
    Book *book = book_form();
	Book *temp = book;
	char chioce;
    book_read(book);
    pack_admin_title(temp, book);

    while(1){
        printf("\t请选择功能：");
        chioce = getch();
        fflush(stdin);
        switch(chioce){
            case 'a': // 图书录入
                pack_insert(book);
                book_write(book);
                break;
            case 'b': // 查看图书信息
                book_inspect(temp, book);
                break;
            case 'c': // 查找图书
                pack_seek(book);
                break;
            case 'd': // 删除图书
                pack_delete(temp, book);
                book_write(book);
                break;
            case 'e': // 修改图书信息
                book_alter(temp, book);
                book_write(book);
                break;
            case 'f': // 入库
                add_stock(temp, book);
                book_write(book);
                break;
            case 'g': // 出库
                reduce_stock(temp, book);
                book_write(book);
                break;
            case 'h': //排序
                sort_of_bookname(book);
                break;
            case 'i': // 退出
                book_unloose(book);
                exit(0);
            case 72:  //返回首页
                pack_begin(&temp, book);
                break;
            case 75: // 上一页
                pack_up(&temp, book);
                break;
            case 77: // 下一页
                pack_down(&temp, book);
                break;
            default:
                break;
        }
        pack_admin_title(temp, book);
    }
    book_unloose(book);
}
//--------------------------登录功能------------------------
/** 管理员登录模块 */
void pack_admin_landing(){
    static int k = 1;
    show_manage();
    if(psd_landing() != 1){ //阻止非管理员账号登录
        if(++k > 3){
            printf("\a\n\t密码错误3次，系统强制退出。");
            exit(-1);
        }
        printf("\n\t账号或密码不存在，请重新登录\n\t");
        system("pause");
        pack_admin_landing();
    }
    printf("\n\n\t登录成功！");
    system("pause");
    admin();
}

 /** 借阅登录模块 */
void pack_std_landing() {
    static int k = 1;
    show_manage();
    if(psd_landing() == 0){
        if(++k > 3){
            printf("\a\n\t密码错误3次，系统强制退出。");
            exit(-1);
        }
        printf("\n\t账号或密码不存在，请重新登录\n\t");
        system("pause");
        pack_std_landing();
    }
    fflush(stdin);
    printf("\n\n\t登录成功！");
    system("pause");
    student();
}

 /** 输出登录输出页面 */
void show_manage(){
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t    账 号 登 录\t\t\t\t|\n");
    printf("\t"H"\n");
}

 /** 输入密码验证 */
int psd_landing(){
	char account[20];       //待验证的账号
    char password[20];      //待验证的密码
	int i, n;
	char ch;
    printf("\n\t请输入账号：");
    gets(account);
    printf("\n\t请输入密码：");
    for(i = 0; (ch = getch()) != 13; i++ ){
        printf("*");
        password[i] = ch;
    }password[i] = '\0';
    n = psd_validate(account, password);
    return n;
 }

 /** 从文件中获取账号密码并验证 */
int psd_validate(char *account, char *password){
	struct _user{
        char account[20];
        char password[20];
        int privilege;  //权限 1-管理员 2-学生 
	}user[N];
	FILE *fp = NULL;
    int i = 0, j;
    if((fp = fopen("user.txt", "r")) == NULL){
        printf("\t文件打开失败！\n");
        exit(-1);
    }
    while(!feof(fp)){
        fscanf(fp, "%s %s %d", user[i].account, user[i].password, &user[i].privilege);
        i++;
    }
    fclose(fp);
    for(j = 0 ; j < i; j++){
        if( (strcmp(user[j].account,  account) == 0) &&
        		(strcmp(user[j].password, password) == 0) ) {
        	return user[j].privilege;    //返回权限
		}
    }
	return 0;
}
//------------------------end----------------------------

//--------------------------注册--------------------------

void show_login(){
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t    账 号 注 册\t\t\t\t|\n");
    printf("\t"H"\n");
}

int obtain_account(char account[N][20]){
    char password[20];
    int data;
    int i = 0;
    FILE *fp;
    if((fp = fopen("user.txt", "r")) == NULL){
        printf("文件打开失败！\n");
        exit(-1);
    }
    while(!feof(fp)){
        fscanf(fp, "%s %s %d", account[i], password, &data);
        i++;
    }
    fclose(fp);
    if(i < N)
        return 1;
    return 0;
}

void login(){
    char account[20];
    char password[20];
    char faccount[N][20]; //从文件获取已有的账号
    char ch;
    FILE *fp;
    int i = 0;
    show_login();
    if(obtain_account(faccount) == 0){
        printf("\t账号已满注册失败，");
        system("pause");
        exit(-1);
    }
    printf("\n\t请输入账号：");
    gets(account);
    while(account[i])
        i++;
    if(i < 6){
        printf("\n\t输入的账号要大于6位，");
        system("pause");
        login();
    }
    for(i = 0; i < N; i++){
        if( strcmp(faccount[i], account) == 0 ){
            printf("\n\t账号已存在，");
            system("pause");
            login();
        }
    }
    printf("\n\t请输入密码：");
    for(i = 0; (ch = getch()) != 13; i++ ){
        printf("*");
        password[i] = ch;
    }password[i] = '\0';
    i = 0;
    while(password[i])
        i++;
    if(i < 6){
        printf("\n\n\t密码要大于六位数,");
        system("pause");
		login();
    }
    if((fp = fopen("user.txt", "a+")) == NULL){
        printf("\t文件打开失败！\n");
        exit(-1);
    }
    fprintf(fp, "\n%s %s %d", account, password, 2);
    printf("\n\n\t注册成功！");
    system("pause");
    fclose(fp);
    denglu();
}
//------------------------输出菜单函数---------------------
/** 打包标题 */
void pack_admin_title(Book *temp, Book *book){
    system("cls");
    show_admin(book);
    book_display(temp, book);
    show_admin_meneu();
}

/** 输出主页面 */
void show_landing(){
    printf("\t"H"\n");
    printf("\t|\t\t欢迎使用图书管理系统\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t|\t\t\t\t\t\t\t|\n");
    printf("\t|\t\t1、学 生 登 录\t\t\t\t|\n");
    printf("\t|\t\t2、管 理 员 登 录\t\t\t|\n");
	printf("\t|\t\t3、账 号 注 册\t\t\t\t|\n");
    printf("\t|\t\t4、修 改 密 码\t\t\t\t|\n");
    printf("\t|\t\t5、退     出\t\t\t\t|\n");
    printf("\t|\t\t\t\t\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t请选择,输入1-5键：");
}

/** 输出标题 */
void show_admin(Book *book){
    int n = show_num(book);
    printf("\t"H"\n");
    printf("\t|\t\t     图书管理系统\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t|   当前库存：\t\t\t\t\t%d\t|\n",n);
    printf("\t"H"\n");
    printf("\t|\t书名\t\t作者\t\t价格\t\t|\n");
}

/** 输出菜单 */
void show_admin_meneu(){
    printf("\t|\t\t\t\t\t\t\t|\n");
    printf("\t|    ← 上一页\t\t↑首页 \t\t下一页→\t|\n");
    printf("\t"H"\n");
    printf("\t| a、图书录入 \t    b、查看图书信息 \tc、查找图书\t|\n");
    printf("\t| d、删除图书 \t    e、修改图书信息 \tf、入库\t\t|\n");
    printf("\t| g、出    库 \t    h、排序(按书名) \ti、退出\t\t|\n");
    printf("\t"H"\n");

}

/** 主菜单输出图书部分信息 */
void show_books(Book *book){
    printf("\t%-15s %-15s %-5.2f\t\t|\n",book->book_name,book->author,book->price);
}
//--------------------------------end----------------------------

//--------------------------查找图书功能-------------------------------
/** 打包查找图书 */
void pack_seek(Book *book){
    show_seek();
    book_seek(book);
}

/** 查找图书时输出 */
void show_seek(){
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t\t查找图书\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t请输入要查找的书名：");
}

/** 查找图书 */
void book_seek(Book *book){
    Book *temp = book->next;
    char book_name[20];
    gets(book_name);
    while(temp != book){
        if(strcmp(temp->book_name, book_name) == 0)
            break;
        temp = temp->next;
    }
    if(temp == book){
        printf("\n\t未找到相关书籍,");
        system("pause");
        return ;
    }
    printf("\t查找的图书信息如下：\n");
    show_book(temp);
    system("pause");

}
//--------------------------------end-----------------------------------

//-------------------------查看图书信息功能---------------------------
/** 查看图书信息 */
void book_inspect(Book *temp, Book *book){
    Book *tep = NULL;
    printf("[查看图书信息]\n\t请输入图书序号：");
    tep = dingwei(temp, book);
    if(tep == NULL)
        return ;
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t\t查看图书\t\t\t|\n");
    printf("\t"H"\n");
    show_book(tep);
    system("pause");
}

/** 输出图书信息 */
void show_book(Book *book){
    printf("\t"H"\n");
    printf("\t|\t\t书  名：\t%-20s\t|\n",book->book_name);
    printf("\t|\t\t作  者：\t%-20s\t|\n",book->author);
    printf("\t|\t\tISBN号：\t%-20s\t|\n",book->isbn);
    printf("\t|\t\t出版社：\t%-20s\t|\n",book->publisher);
    printf("\t|\t\t出版年份：\t%-6d\t\t\t|\n",book->pub_data);
    printf("\t|\t\t价  格：\t%-6.2f\t\t\t|\n",book->price);
    printf("\t|\t\t库  存：\t%-6d\t\t\t|\n",book->stock);
    printf("\t"H"\n\t");
}
//----------------------------end-------------------------------

//-------------------------图书录入功能---------------------------
/** 打包添加图书 */
void pack_insert(Book *book){
    show_insert();
    book_insert(book);
    printf("\t图书录入成功，");
    system("pause");
}

/** 图书录入时输出 */
void show_insert(){
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t\t增加图书\t\t\t|\n");
    printf("\t"H"\n");
}

/** 增加图书 */
void book_insert(Book *book){
    Book *temp = book;
    Book *p;
    p = (Book *)malloc(sizeof(*p));
    book_construct(p);   //调用输入函数
    printf("\t请输入数量：");
    scanf("%d", &p->stock); //库存
    //尾部插入
    while(temp->next != book)
        temp = temp->next;
    p->next = temp->next;
    temp->next = p;
    p->prev = temp;
    p->next->prev = p;
}
//----------------------------end---------------------------------

//--------------------------删除图书功能--------------------------
/** 打包删除图书 */
void pack_delete(Book *temp, Book *book){
	int n;
    printf("[删除图书]\n\t请输入图书序号：");
    n = book_delete(temp, book);
    if(n == 1){
        printf("\t删除成功,");
        system("pause");
    }
    if(n == -1){
        printf("\t删除失败,");
        system("pause");
    }
}

/** 删除图书 */
int book_delete(Book *temp, Book *book){
    Book *p = NULL;
    Book *tep = NULL;
    char chioce;
    tep = dingwei(temp, book);
    fflush(stdin);
    if(tep == NULL)
        return 0;

    system("cls");
	printf("\t"H"\n");
    printf("\t|\t\t\t删除图书\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t要删除的信息如下：\n");
    show_book(tep);
    printf("\t是否确认删除(Y/N)：");
    chioce = getchar();
    if(chioce == 'Y' || chioce == 'y'){
        p = tep;
        tep->prev->next = p->next;
        p->next->prev = tep->prev;
        free(p);
        return 1;
    }
    return -1;
}
//----------------------------end----------------------------

//------------------------修改图书信息功能--------------------
/** 修改图书信息 */
void book_alter(Book *temp ,Book *book){
    Book *tep = NULL;
    printf("[修改图书信息]\n\t请输入图书序号：");
    tep = dingwei(temp, book);
    if(tep == NULL)
        return ;
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t\t修改图书\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t要修改的图书信息\n");
    show_book(tep);
    printf("请输入图书信息：\n");
	printf("\t"H"\n");
    book_construct(tep);
    printf("\t修改成功,");
    system("pause");
}
//----------------------------end------------------------

//---------------------------文件操作-------------------------
/** 从文件读取数据 */
void book_read(Book *book){
    FILE *fp = NULL;
    Book *p = NULL, *temp = book;
    if((fp = fopen("books.txt", "r")) == NULL){
        default_book();
        fp = fopen("books.txt", "r");
    }
    while( !feof(fp) ){
        p = (Book *)malloc(sizeof(*p));
        fscanf(fp,"%s%s", p->book_name, p->author);
        fscanf(fp,"%s%s", p->isbn, p->publisher);
        fscanf(fp,"%d", &p->pub_data);
        fscanf(fp,"%f%d",&p->price, &p->stock);

        while(temp->next != book)
            temp = temp->next;
        p->next = temp->next;
        temp->next = p;
        p->prev = temp;
        p->next->prev = p;
    }
    fclose(fp);
}

/** 将数据导入文件 */
void book_write(Book *book){
    FILE *fp = NULL;
    Book *temp = book;
    int falg = 0;
    if((fp = fopen("books.txt", "w")) == NULL){
        printf("\t文件打开失败！\n");
        exit(-1);
    }
    while(temp->next != book){
        if( falg )
            fputc('\n', fp);
        fprintf(fp, "%s %s %s %s %d %f %d",temp->next->book_name, temp->next->author,temp->next->isbn,
                temp->next->publisher, temp->next->pub_data, temp->next->price, temp->next->stock);
        temp = temp->next;
        falg = 1;
    }
    fclose(fp);
}
//------------------------------end------------------------------

//-----------------------------学生借阅功能-------------------------
void pack_std_title(Book *temp, Book *book) {
    system("cls");
    show_std(book);
    book_display(temp, book);
    show_std_meneu();
}

void show_std(Book *book){
    int n = show_num(book);
    printf("\t"H"\n");
    printf("\t|\t\t     图书借阅系统\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t|   当前库存：\t\t\t\t\t%d\t|\n",n);
    printf("\t"H"\n");
    printf("\t|\t书名\t\t作者\t\t价格\t\t|\n");
}

 void show_std_meneu() {
    printf("\t|\t\t\t\t\t\t\t|\n");
    printf("\t|    ← 上一页\t\t↑ 首页\t\t下一页→\t|\n");
    printf("\t"H"\n");
    printf("\t| a、查找图书\t\tb、借书 \tc、查看图书信息\t|\n");
    printf("\t| d、已借图书\t\te、还书 \tf、退出 \t|\n");
    printf("\t"H"\n");
 }
//-----------------------------end-------------------------------

//------------------------ 图书排序功能---------------------------

void sort_of_bookname(Book *book){
    Book *i = NULL, *j = NULL;
    Book *follow = NULL;
    Book *temp = book;
    char book_name[20];
    temp->next->prev = temp->prev;  //第一个结点不参加排序
    for(i = temp->next; i->prev != i; i = i->prev){
        strcpy(book_name,i->prev->book_name);
        follow = i;
        for(j = i->prev; j != i; j = j->prev){
            if(strcmp(j->prev->book_name,book_name) < 0){
                strcpy(book_name ,j->prev->book_name);
                follow = j;
            }
        }
        temp->next = follow->prev;
        temp = follow->prev;
        follow->prev = temp->prev;
    }
    temp->next = i;  //剩下最后一个数据
    i->next = book;
    book->prev = i;  //先让第一个结点的prev指向最后一个数据
    //重新将prev指针连起来
    temp = book;
    while(temp->next != book){
        temp->next->prev = temp;
        temp = temp->next;
    }
}

//------------------------入库--------------------------
void add_stock(Book *temp, Book *book){
    Book *tep = NULL;
    int num = 0;
    printf("[入库]\n\t请输入图书序号：");
    tep = dingwei(temp, book);
    if(tep == NULL)
        return ;
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t\t 入   库  \t\t\t|\n");
    printf("\t"H"\n");
    printf("\t选择的图书信息如下：\n");
    show_book(tep);
    printf("\t请输入入库数量：");
    scanf("%d", &num);
    tep->stock += num;
    printf("\t入库成功！");
    system("pause");
}

//------------------------出库--------------------------
void reduce_stock(Book *temp, Book *book){
    Book *tep = NULL;
    int num = 0;
    printf("[出库]\n\t请输入图书序号：");
    tep = dingwei(temp, book);
    if(tep == NULL)
        return ;
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t\t 出   库  \t\t\t|\n");
    printf("\t"H"\n");
    printf("\t选择的图书信息如下：\n");
    show_book(tep);
    printf("\t请输入出库数量：");
    scanf("%d", &num);
    if(tep->stock < num){
        printf("\t出库失败，库存不足！");
        system("pause");
        return ;
    }
    tep->stock -= num;
    printf("\t出库成功！");
    system("pause");
}
//------------------------借阅-------------------------
void borrow_book(Book *temp, Book *book){
    Book *tep = NULL;
    char chioce;
    int num = 1;
    printf("[借阅]\n\t请输入图书序号：");
    tep = dingwei(temp, book);
    if(tep == NULL)
        return ;
    system("cls");
    printf("\t"H"\n");
    printf("\t|\t\t    借 阅 图 书\t\t\t\t|\n");
    printf("\t"H"\n");
    printf("\t选择的图书信息如下：\n");
    show_book(tep);
    printf("\t是否确认借阅(Y/N)：");
    getchar();
    chioce = getchar();
    if((chioce == 'Y' || chioce == 'y') && tep->stock > 0){
        tep->stock -= num;
        printf("\t借阅成功！");
        system("pause");
    }
    else{
        if(!(tep->stock > 0))
            printf("\t库存不足，借阅失败！");
        else
            printf("\t借阅失败！");
        system("pause");
        return ;
    }

}
//----------------------定位图书------------------------
Book* dingwei(Book *temp, Book *book){
    int i, chioce;
    scanf("%d", &chioce);
    for(i = 0; i < chioce; i++){
        temp = temp->next;
        if(chioce > 9 || temp == book){
            printf("\t未找到相关书籍，");
            system("pause");
            return NULL;
        }
    }
    return temp;
}
//------------------------ 上一页 -----------------------
void pack_up(Book **temp, Book *book){
    Book *tep = *temp;
    int flag = 1, i;
    for(i = 0; i < 9; i++){
        tep = tep->prev;
        if(tep == book->prev){
            flag = 0;
            break;
        }
    }
    if(flag)
        *temp = tep;
}

//------------------------ 首页 -----------------------
void pack_begin(Book **temp, Book *book){
    *temp = book;
}

//------------------------ 下一页-----------------------
void pack_down(Book **temp, Book *book){
    Book *tep = *temp;
    int flag = 1, i;
    for(i = 0; i < 9; i++){
        tep = tep->next;
        if(tep == book){
            flag = 0;
            break;
        }
    }
    if(flag)
        *temp = tep;
}

/** 遍历图书 */
void book_display(Book *temp, Book *book){
    int i = 1;
    while(temp->next != book && i < 10){
        printf("\t| %d、",i++);
        show_books(temp->next);
        temp = temp->next;
    }
}

 /** 构造函数 */
void book_construct(Book *book){
    fflush(stdin);
    printf("\t请输入书名：");
    gets(book->book_name);
    printf("\t请输入作者：");
    gets(book->author);
    printf("\t请输入ISBN号：");
    gets(book->isbn);
    printf("\t请输入出版社：");
    gets(book->publisher);
    fflush(stdin);
    printf("\t请输入时间：");
    scanf("%d", &book->pub_data);
    printf("\t请输入价格：");
    scanf("%f", &book->price);
}

/** 创建默认图书 */
void default_book(){
    FILE *fp = NULL;
    if((fp = fopen("books.txt", "w")) == NULL){
        printf("文件打开失败！\n");
        exit(-1);
    }
    fprintf(fp, "%s %s %s %s %d %f %d", "c语言","老九君", "c123",
            "老九学堂", 2016, 30.0f, 1);
    fclose(fp);
}

/** 创建链表 */
Book* book_form(){
    Book *book;
    book = (Book *)malloc(sizeof(* book));
    if( !book )
        exit(-1);
    book->next = book->prev = book;
    return book;
}

/** 释放链表 */
void book_unloose(Book *book){
    Book *temp = book;
    Book *q;
    while(temp->next != book){
        q = temp->next;
        temp->next = q->next;
        free(q);
    }
    free(book);
}

/** 链表长度 */
int show_num(Book *book){
    Book *temp = book;
    int num = 0;
    while(temp->next != book){
        num++;
        temp = temp->next;
    }
    return num;
}
