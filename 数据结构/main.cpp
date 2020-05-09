#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100		// 最多景点个数
#define M 10		// map
#define INF 999999	// 最大值
#define L "------------------------------------------------------------"

/** 存储顶点信息的结构体 */
typedef struct _vertex {
    int number; // 每个景点有唯一的编号，由系统分配
    int x;      // 顶点横坐标
    int y;      // 顶点纵坐标
    char designation[20];   // 名称
    char synopsis[200];     // 简介
}Vertex;
/** 存储景点信息的结构体 */
typedef struct {
    Vertex vexs[N];     // 顶点信息域
    int edges[N][N];    // 邻接矩阵
    int n;				// 顶点数
}Mgraph;

/** stack */
typedef struct {
    int data[N];
    int top;
}Stack;

void _main(Mgraph g, Stack st, int path[N][N], int dist[N][N]);
void show_plane_title();
void show_plane_content(Mgraph g);
void show_plane_menu();
int show_plane_model(Mgraph g);
void show_grid_content(Mgraph g);
void show_grid_menu();
int show_grid_model(Mgraph g);
void read_file(Mgraph *g);
void update_file(Mgraph g);
void shortest_path(Mgraph g, int path[N][N], int dist[N][N]);
void show_path(Mgraph g, int path[N][N], int dist[N][N], int n1, int n2);
void search(Mgraph g, int search_num);
int judeg(Stack *st, int num);
void all_path(Mgraph g, Stack *st,  int s1, int s2);
void add_side(Mgraph *g, int n);
void del_site(Mgraph *g, int number);
void add_site(Mgraph *g);
void update_edges(Mgraph *g, int n);
void change_msg(Mgraph *g, int n);
int str_lenght(char *s);

int OK = 0; 	// 1：表示path，dist可以使用

int main() {
    Mgraph g;
    Stack st;
    st.top = 0;
    int path[N][N];
    int dist[N][N];	
    read_file(&g);				// 读取文件
	_main(g, st, path, dist);	// 运行程序
    return 0;
}

void _main(Mgraph g, Stack st, int path[N][N], int dist[N][N]) {
	system("cls");
	int k = 1;			// 表示输出的菜单
	int chioce;			// 选择的功能
	int num, n1, n2;	// 选择的编号
	while(1) {
		if(k == 1)
			chioce = show_plane_model(g);
		else
			chioce = show_grid_model(g);
		
		switch(chioce) {
			case 1:		// 切换菜单
				if (k == 1) k = 2;
				else k = 1;
				break;
			case 2:		// 查找景点
				printf("\t请输入景点编号：");
				scanf("%d", &num);
				search(g, num);
				break;
			case 3:		// 添加景点
				add_site(&g);
				update_file(g);
				break;
			case 4:		// 删除景点
				printf("\t请输入景点编号：");
				scanf("%d", &num);
				del_site(&g, num);
				update_file(g);
				break;
			case 5:		// 修改景点
				printf("\t请输入景点编号：");
				scanf("%d", &num);
				change_msg(&g, num-1);
				update_file(g);
				break;
			case 6:		// 最短路径
				printf("\t请输入两景点的编号：");
				scanf("%d%d", &n1, &n2);
				if(n1<1 || n1>g.n || n2<1 || n2>g.n) {
					printf("\t输入的编号有误,");
					system("pause");
					break;
				}
				if(!OK) shortest_path(g, path, dist);
				show_path(g, path, dist, n1-1, n2-1);
				break;
			case 7:		// 全部路径
				printf("\t请输入两景点的编号：");
				scanf("%d%d", &n1, &n2);
				if(n1<1 || n1>g.n || n2<1 || n2>g.n) {
					printf("\t输入的编号有误,");
					system("pause");
					break;
				}
				system("cls");
				printf("\t " L "\n");
				printf("\t|\t\t\t全部路径查询\t\t\t    |\n");
				printf("\t " L "\n");
				all_path(g, &st, n1-1, n2-1);
				printf("\n\t");
				system("pause");
				break;
			case 0:		// 退出程序
				printf("\t");
				exit(0);
			default:
				break;
		}
	}
}

// ----------------------------列表模式菜单------------------------------
/**
 * 开始菜单页输出
 */
void show_plane_title() {
	printf("\t " L "\n");
	printf("\t|\t\t\t校园导游程序\t\t\t    |\n");
	printf("\t " L "\n");
}

/**
 * 内容块输出
 */
void show_plane_content(Mgraph g) {
	char str[50];
	int n = 0;
	printf("\t|   编号      名称\t\t\t简介\t\t    |\n");
	for(int i=0; i < g.n; i++) {
		strncpy(str, g.vexs[i].synopsis, 30);
		if((n = str_lenght(g.vexs[i].synopsis) > 30)) {
			str[30] = '\0';
			strcat(str, "..");
		}
		printf("\t|   %-9d%-15s%-32s|\n", g.vexs[i].number, g.vexs[i].designation, str);
	}
	printf("\t " L "\n");
}

/**
 * 功能块输出
 */
void show_plane_menu() {
	printf("\t| 1、平面模式\t2、查询景点\t3、增加景点\t4、删除景点 |\n");
	printf("\t| 5、修改景点\t6、最短路径\t7、全部路径\t0、退 出    |\n");
	printf("\t " L "\n");
}

/**
 * 列表模式菜单
 */
int show_plane_model(Mgraph g) {
	system("cls");
	show_plane_title();
	show_plane_content(g);
	show_plane_menu();
	int choice;
	printf("\t请选择功能：");
	scanf("%d", &choice);
	return choice;
}
// ---------------------------------end----------------------------------

// ----------------------------平面模式菜单------------------------------
/**
 * 内容块输出
 */
void show_grid_content(Mgraph g) {
	char str[20];       	// 临时存放名称
    char map[M][M][20]; 	// 平面图数组
	char tab[10] = "     ";	// 空格长度
	int i;
    // 初始化平面图
    for(i=0; i < M; i++) {
        for(int j=0; j < M; j++) {
            strcpy(map[i][j], tab);
        }
    }
    // 存在bug：如果xy重复，数组值将被覆盖
    for(i=0; i < g.n; i++) {
        // sprintf: 将数字转换成字符串，并且覆盖前面的字符串
        sprintf(str, "%d", g.vexs[i].number);
        strcat(str, g.vexs[i].designation);
        strcat(str, "  ");
        strcpy(map[g.vexs[i].x][g.vexs[i].y], str);
    }
    // 输出平面图
    for(i=0; i < M; i++) {
		printf("\t");
        for(int j=0; j < M; j++) {
            printf("%s",map[i][j]);
        }
        printf("\n");
    }
}

/**
 * 功能块输出
 */
void show_grid_menu() {
	printf("\t " L "\n");
	printf("\t| 1、列表模式\t2、查询景点\t3、增加景点\t4、删除景点 |\n");
	printf("\t| 5、修改景点\t6、最短路径\t7、全部路径\t0、退 出    |\n");
	printf("\t " L "\n");
}

/**
 * 平面模式菜单
 */
int show_grid_model(Mgraph g) {
	system("cls");
	show_plane_title();
	show_grid_content(g);
	show_grid_menu();
	int choice;
	printf("\t请选择功能：");
	scanf("%d", &choice);
	return choice;
}
// ---------------------------------end----------------------------------

// ------------------------------文件读取操作----------------------------
/**
 * 读取文件信息
 */
void read_file(Mgraph *g) {
    FILE *fv, *fe;
    int n = 0;  // 读取的顶点个数
	// 打开文件
    if ((fv = fopen("vexs.txt", "r")) == NULL || (fe = fopen("edges.txt", "r")) == NULL) {
        printf("文件打开失败\n");
        exit(-1);
    }
    // 读取顶点信息
    while (!feof(fv)) {
        fscanf(fv, "%d%d%d%s%s", &g->vexs[n].number, &g->vexs[n].x, &g->vexs[n].y,
               g->vexs[n].designation, g->vexs[n].synopsis);
        n++;
    }
    g->n = n;
    // 初始化矩阵
    int x, y, val;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j)
                g->edges[i][j] = 0;
            else
                g->edges[i][j] = INF;
        }
    }
	// 读取链接矩阵
    while(!feof(fe)) {
        fscanf(fe, "%d%d%d", &x, &y, &val);
        g->edges[x][y] = val;
        g->edges[y][x] = val;
    }
    fclose(fv);
    fclose(fe);
}

/**
 * 更新文件的信息
 */
void update_file(Mgraph g) {
    FILE *fv, *fe;
	int i;
	// 打开文件
	if ((fv = fopen("vexs.txt", "w")) == NULL || (fe = fopen("edges.txt", "w")) == NULL) {
        printf("文件打开失败\n");
        exit(-1);
    }
    // 更新顶点信息
    fprintf(fv, "%d %d %d %s %s", g.vexs[0].number, g.vexs[0].x, g.vexs[0].y, g.vexs[0].designation, g.vexs[0].synopsis);
    for(i = 1; i < g.n; i++) {
        fprintf(fv, "\n%d %d %d %s %s", g.vexs[i].number, g.vexs[i].x, g.vexs[i].y,
              g.vexs[i].designation, g.vexs[i].synopsis);
    }
    // 更新矩阵信息
    int flag = 0;				// 判断是否在文件中加换行，第一次不加
    for(i = 0; i < g.n; i++) {
        for(int j=i+1; j < g.n; j++) {
            if(g.edges[i][j] != 0 && g.edges[i][j] != INF) {
                if(flag) fputc('\n',fe);
                fprintf(fe, "%d %d %d", i, j, g.edges[i][j]);
                flag = 1;
            }
        }
    }
    fclose(fv);
    fclose(fe);
}

// ---------------------------------end----------------------------------

// -------------------------------功能函数-------------------------------

/**
 * 增加景点信息
 */
void add_site(Mgraph *g) {
	system("cls");
	printf("\t " L "\n");
	printf("\t|\t\t\t添加景点信息\t\t\t    |\n");
	printf("\t " L "\n");
    int n = g->n;
    g->vexs[n].number = n+1;
    fflush(stdin);
    printf("\t请输入景点名称：");
    scanf("%s", g->vexs[n].designation);
    fflush(stdin);
    printf("\t请输入景点的坐标x y：");
    scanf("%d%d", &g->vexs[n].x, &g->vexs[n].y);
    fflush(stdin);
    printf("\t请输入景点的描述：");
    scanf("%s", g->vexs[n].synopsis);
	fflush(stdin);
    add_side(g, g->vexs[n].number);
    printf("\t添加景点成功,");
    g->n++;
	system("pause");
}

/**
 * 按编号删除景点
 * @param number 要删除的编号
 */
void del_site(Mgraph *g, int number) {
	char c;
	system("cls");
	printf("\t " L "\n");
	printf("\t|\t\t\t添加景点信息\t\t\t    |\n");
	printf("\t " L "\n");
	// 输入的编号不存在
    if(number > g->n || number < 1) {
        printf("\t输入的编号有误,");
		system("pause");
        return ;
    }
	printf("\n\t编号：%d\n", g->vexs[number-1].number);
	printf("\t名称：%s\n", g->vexs[number-1].designation);
	printf("\t简介：%s\n\n", g->vexs[number-1].synopsis);
	printf("\t是否确认删除(y/n)：");
	fflush(stdin);
	c = getchar();
	// 取消删除
	if(c == 'n') {
		printf("\t取消删除景点,");
		system("pause");
		return ;
	}
    // 删除顶点
    for(int j=number; j < g->n; j++) {
        g->vexs[j] = g->vexs[j+1];
        g->vexs[j].number -= 1;
    }
    // 更新边的关系
    update_edges(g, number);
    g->n--;
	printf("\n\t删除成功，");
	system("pause");
}

/**
 * 更改景点信息
 * @param n 要更改的顶点的信息
 */
void change_msg(Mgraph *g, int n) {
    int choice = 0;
	system("cls");
	printf("\t " L "\n");
	printf("\t|\t\t\t更改景点信息\t\t\t    |\n");
	printf("\t " L "\n");
    printf("\t请选择修改该顶点的什么信息: \n");
    while(choice != 5) {
        printf("\t1:名称 2：坐标 3：描述 4：修改/增加边关系: 5: 退出：");
        scanf("%d", &choice);
		fflush(stdin);
        switch (choice) {
            case 1:		// 修改名称
                printf("\t请输入名称：");
                scanf("%s", g->vexs[n].designation);
				fflush(stdin);
                break;
            case 2:		// 修改坐标
                printf("\t请输入坐标(x,y)：");
                scanf("(%d,%d)", &g->vexs[n].x, &g->vexs[n].y);
				fflush(stdin);
                break;
            case 3:		// 修改描述
                printf("\t请输入描述：");
                scanf("%s", g->vexs[n].synopsis);
				fflush(stdin);
                break;
            case 4:		// 修改变关系
                add_side(g, n+1);
                break;
            case 5:		// 退出修改
				printf("\n\t修改成功,");
				system("pause");
                return ;
            default:
                break;
        }
    }
}

/**
 * 查询各地点的信息
 * 按编号查询
 */
void search(Mgraph g, int search_num) {
	system("cls");
	printf("\t " L "\n");
	printf("\t|\t\t\t景点信息查询\t\t\t    |\n");
	printf("\t " L "\n");
	if(search_num < 1 || search_num > g.n) {
		printf("\t未找到相关地点,");
	} else {
		printf("\n\t编号：%d\n", g.vexs[search_num-1].number);
		printf("\t名称：%s\n", g.vexs[search_num-1].designation);
		printf("\t坐标：%d %d\n", g.vexs[search_num-1].x, g.vexs[search_num-1].y);
		printf("\t简介：%s\n\n\t", g.vexs[search_num-1].synopsis);
	}
	system("pause");
}

/**
 * 求两点间最短路径集合
 * @param path 到各顶点的最短路径向量
 * @param dist 到各顶点最短路径长度向量
 */
void shortest_path(Mgraph g, int path[N][N], int dist[N][N]) {
    // 初始化
    for(int i=0; i < g.n; i++) {
        for(int j=0; j < g.n; j++) {
            dist[i][j] = g.edges[i][j];
            if(i!=j && dist[i][j] < INF)
                path[i][j] = j;
            else
                path[i][j] = -1;
        }
    }
    // 递推求解每一对顶点将的最短距离
    for(int k=0; k < g.n; k++) {
        for(int i=0; i < g.n; i++){
            for(int j=0; j < g.n; j++) {
                if(dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j]; //取小者
                    path[i][j] = path[i][k];    //改Vi的后继
                }
            }
        }
    }
	OK = 1;		// 将OK置1，表示dist，path数组可用
}

/**
 * 全部路径 => 深度优先搜索
 * @param s1,s2 按编号查找两个地点的全部路径
 */
void all_path(Mgraph g, Stack *st,  int s1, int s2) {
	// 将起始景点加入st中
    if(st->top == 0) st->data[st->top++] = s1;
	// 得到顶端的元素
    int s = st->data[st->top-1];
	// 如果顶端元素时结束的景点，则输出这条路径
    if(s == s2) {
		printf("\t");
        for(int i=0; i < st->top-1; i++) {
            printf("%d -> ", st->data[i]+1);
        }
        printf("%d\n", s2+1);
        st->top--;
        return ;
    }
	// 递归遍历
    for(int i = 0; i < g.n; i++) {
        if(s == i) continue;
        if(g.edges[s][i] != INF && !judeg(st, i)) {
            st->data[st->top++] = i;
            all_path(g, st, s1, s2);
        }
    }
    st->top--;
}

// ---------------------------------end----------------------------------

// -------------------------------工具函数-------------------------------
/**
 * 获取字符串长度
 */
int str_lenght(char *s) {
	int i=0;
	while(*(s+i))
		i++;
	return i;
}

/**
 * 判断num是否在st中
 * @return 存在返回1
 */
int judeg(Stack *st, int num) {
    for(int i=0; i < st->top; i++) {
        if(st->data[i] == num)
            return 1;
    }
    return 0;
}

/**
 * 更新/新增两条边的关系
 * @param n 指定的一条边
 */
void add_side(Mgraph *g, int n) {
    printf("\t请输入该景点%d与其他景点x的距离s\n\t格式：x s, 0 0退出输入：", n);
    int x, s;
	scanf("%d%d", &x, &s);
    while(x > 0 && s > 0) {
        g->edges[n-1][x-1] = s;
        g->edges[x-1][n-1] = s;
        printf("\t格式：x s, 0 0退出输入：");
		scanf("%d%d", &x, &s);
		OK = 0;		// 将OK置0，表示dist，path数组不可用
    }
}

/**
 * 更新矩阵信息
 * @param n 从n开始更新
 */
void update_edges(Mgraph *g, int n) {
    int x, y;
    for(int i=n; i < g->n; i++) {
        for(int j=i+1; j < g->n; j++) {
            if(i < n && j < n) continue;
			// 由于某景点被删除，其后面的编号要往前更改
            x = i < n ? i : i-1;
            y = j < n ? j : j-1;
            g->edges[x][y] = g->edges[i][j];
            g->edges[y][x] = g->edges[j][i];
        }
    }
}

/**
 * 输出最短路径
 */
void show_path(Mgraph g, int path[N][N], int dist[N][N], int n1, int n2) {
    int k;
	system("cls");
	printf("\t " L "\n");
	printf("\t|\t\t\t最短路径查询\t\t\t    |\n");
	printf("\t " L "\n");
	printf("\n\tV%d到V%d的最短长度:",n1+1,n2+1);
    printf("%d\t",dist[n1][n2]);		//输出Vi到Vj的最短路径长度
	k = path[n1][n2];					//取路径上Vi的后续Vk
    if(k == -1){
		printf("\n\tThere is no path between V%d and V%d\n",n1+1,n2+1);   //路径不存在
	}else{
		printf("\n\t最短路径为:");
		printf("(V%d",n1+1);			//输出Vi的序号i
		while(k != n2){					//k不等于路径终点j时
			printf(" -> V%d",k+1);		//输出k
			k=path[k][n2];				//求路径上下一顶点序号
		}
		printf(" -> V%d)\n",n2+1);		//输出路径终点序号
	}
	printf("\n\t");
	system("pause");
}
