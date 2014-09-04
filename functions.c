#include "header.h"
static complex x[NUM];
static complex x_raw[NUM];
static int M;
static GLfloat d[NUM];
void add(complex a,complex b,complex *c)   /*复数加法*/
{
    c->real=a.real+b.real;
    c->img=a.img+b.img;
}
void mul(complex a,complex b,complex *c)   /*复数乘法*/
{
  c->real=a.real*b.real-a.img*b.img;
  c->img=a.real*b.img+a.img*b.real;
}
void sub(complex a,complex b,complex *c)    /*复数减法*/
{
  c->real=a.real-b.real;
  c->img=a.img-b.img;
}
void divi(complex a,complex b,complex *c)   /*复数除法*/
{
  c->real=(a.real*b.real+a.img*b.img)/(b.real*b.real+b.img*b.img);
  c->img=(a.img*b.real-a.real*b.img)/(b.real*b.real+b.img*b.img);
}
complex *initW()                                /*生成旋转因子*/
{
  int i;
  complex *W;
  W=(complex *)malloc(sizeof(complex)*M);
  for(i=0;i<M;i++)
  {
  W[i].real=cos(2*PI/M*i);
  W[i].img=-1*sin(2*PI/M*i);
  }
  return W;
}
void sort()                                 /*将x(n)按照码位排序*/
  {
  complex temp;
  unsigned short i=0,j=0,k=0;
  double t;
  for(i=0;i<M;i++)
  {
  k=i;j=0;
  t=(log(M)/log(2));
  while((t--)>0)
  {
  j=j<<1;
  j|=(k&1);
  k=k>>1;
  }
  if(j>i)
  {
  temp=x[i];
  x[i]=x[j];
  x[j]=temp;
  }
  }
}
void cal_fft(complex *W)                                /*进行FFT变换*/
{
  int i=0,j=0,k=0,l=0;
  complex up,down,product;
  sort();
  for(i=0;i<log(M)/log(2);i++)
  {
  l=1<<i;
  for(j=0;j<M;j+=   2*l   )
  {
  for(k=0;k<l;k++)
  {
  mul(x[j+k+l],W[M*k/2/l],&product);
  add(x[j+k],product,&up);
  sub(x[j+k],product,&down);
  x[j+k]=up;
  x[j+k+l]=down;
  }
  }
  }
}
void fft()
{
    int i,j;                                                          /*输入参数模块*/
    float fc,f[10],fs;
    for(i=0;i<=9;i++)
    {
        f[i]=0;
    }
    f[1]=1;
    printf("输入正弦波频率： ");
    scanf("%f",&fc);
    printf("\n是否含有高次谐波(Y/N)： ");
    getchar();
    char c=getchar();
    if(c=='Y'||c=='y')
    {
        printf("\n开始输入谐波相对强度：(-1 To Skip)\n");
        for(i=2;i<=9;i++)
        {
            printf("\n%d次谐波强度： ",i);
            scanf("%f",&f[i]);
            if(f[i]==-1)break;
        }
    }
    printf("\n输入采样点数(2的n次方)： ");
    scanf("%d",&M);
    printf("\n输入采样频率： ");
    scanf("%f",&fs);
    for(i=0;i<M;i++)                                                /*生成信号采样序列模块*/
    {
        double sum=0;
        for(j=1;j<=9;j++)
        {
            sum=sum+f[j]*sin(2*PI*j*fc*i/fs);
        }
        x[i].real=sum;
        x[i].img=0;
        x_raw[i].real=sum;
        x_raw[i].img=0;
    }
    complex *W=initW();
    cal_fft(W);
    printf("FFT计算结果如下:\n");                                     /*输出计算结果模块*/
    for(i=0;i<M;i++)
    {
      printf("%.4f",x[i].real);
      if(x[i].img>=0.0001)
      printf("+%.4fj\n",x[i].img);
      else   if(fabs(x[i].img)<0.0001)
      printf("\n");
      else
      printf("%.4fj\n",x[i].img);
    }
    for(i=0;i<M;i++)
    {
        x[i].mo=sqrt(x[i].real*x[i].real+x[i].img*x[i].img);
        x_raw[i].mo=x_raw[i].real;
    }
}
double max(complex *x)
{
    int i;
    double max=0;
    for(i=0;i<=M;i++)
    {
        if(x[i].mo>max)max=x[i].mo;
    }
    return max;
}
void display1()
{
    int i;
    GLfloat x_d=(double)2/M;
    GLfloat x_p=-1;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f,-1.0f);
    glVertex2f(1.0f,-1.0f);
    glEnd();
    glColor3f(0.0f,0.0f,1.0f);
    double max_=max(x);
    glBegin(GL_LINES);
    for(i=0;i<=M;i++)
    {
        glVertex2f(x_p,-1.0f);
        glVertex2f(x_p,2*x[i].mo/max_-1);
        x_p+=x_d;
    }
    glEnd();
    glFlush();
}
void display2()
{
    int i;
    GLfloat x_d=(double)2/M;
    GLfloat x_p=-1;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f,0.0f);
    glVertex2f(1.0f,0.0f);
    glEnd();
    glColor3f(0.0f,0.0f,1.0f);
    double max_=max(x_raw);
    glBegin(GL_LINES);
    for(i=0;i<=M;i++)
    {
        glVertex2f(x_p,0.0f);
        glVertex2f(x_p,x_raw[i].mo/max_);
        x_p+=x_d;
    }
    glEnd();
    glFlush();
}
int draw1(int argc,char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(4.7*M>=1000?4.7*M:1000, 600);
    glutCreateWindow("After FFT");
    glutDisplayFunc(&display1);
    glutCreateWindow("Signal");
    glutDisplayFunc(&display2);
    glutMainLoop();
    return 0;
}
void draw()
{
    draw1(NULL,NULL);
}
