#include <iostream>   
#include <fstream>
#include <stdlib.h>      
#include <cmath>        
using namespace std;
#ifndef _In_opt_
  #define _In_opt_ 
#endif
#ifndef _Out_
  #define _Out_ 
#endif
 
class Matrix
{
private:
    unsigned  row, col, size;
    double *pmm;//数组指针    
public:
    Matrix(unsigned r, unsigned c) :row(r), col(c)//非方阵构造     
    {
        size = r*c;
        if (size>0)
        {
            pmm = new double[size];
            for (unsigned j = 0; j<size; j++) //init      
            {
                pmm[j] = 0.0;
            }
        }
        else
            pmm = NULL;
    };
    Matrix(unsigned r, unsigned c, double val ) :row(r), col(c)// 赋初值val  
    {
        size = r*c;
        if (size>0)
        {
            pmm = new double[size];
            for (unsigned j = 0; j<size; j++) //init      
            {
                pmm[j] = val;
            }
        }
        else
            pmm = NULL;
    };
    Matrix(unsigned n) :row(n), col(n)//方阵构造      
    {
        size = n*n;
        if (size>0)
        {
            pmm = new double[size];
            for (unsigned j = 0; j<size; j++) //init      
            {
                pmm[j] = 0.0;
            }
        }
        else
            pmm = NULL;
    };
    Matrix(const Matrix &rhs)//拷贝构造   
    {
        row = rhs.row;
        col = rhs.col;
        size = rhs.size;
        pmm = new double[size];
        for (unsigned i = 0; i<size; i++)
            pmm[i] = rhs.pmm[i];
    }
 
    ~Matrix()//析构  
    {
        if (pmm != NULL)
        {
            delete[]pmm;
            pmm = NULL;
        }
    }
 
    Matrix  &operator=(const Matrix&);  //如果类成员有指针必须重写赋值运算符，必须是成员      
    friend istream &operator>>(istream&, Matrix&);
    
    friend ofstream &operator<<(ofstream &out, Matrix &obj);  // 输出到文件
    friend ostream &operator<<(ostream&, Matrix&);          // 输出到屏幕
    friend Matrix  operator+(const Matrix&, const Matrix&);
    friend Matrix  operator-(const Matrix&, const Matrix&);
    friend Matrix  operator*(const Matrix&, const Matrix&);  //矩阵乘法
    friend Matrix  operator*(double, const Matrix&);  //矩阵乘法
    friend Matrix  operator*(const Matrix&, double);  //矩阵乘法
 
    friend Matrix  operator/(const Matrix&, double);  //矩阵 除以单数
 
    Matrix multi(const Matrix&); // 对应元素相乘
    Matrix mtanh(); // 对应元素相乘
    unsigned Row()const{ return row; }  
    unsigned Col()const{ return col; }
    Matrix getrow(size_t index); // 返回第index 行,索引从0 算起
    Matrix getcol(size_t index); // 返回第index 列
 
    Matrix cov(_In_opt_ bool flag = true);   //协方差阵 或者样本方差   
    double det();   //行列式    
    Matrix solveAb(Matrix &obj);  // b是行向量或者列向量
    Matrix diag();  //返回对角线元素    
    //Matrix asigndiag();  //对角线元素    
    Matrix T()const;   //转置    
    void sort(bool);//true为从小到大          
    Matrix adjoint();
    Matrix inverse();
    void QR(_Out_ Matrix&, _Out_ Matrix&)const;
    Matrix eig_val(_In_opt_ unsigned _iters = 1000);
    Matrix eig_vect(_In_opt_ unsigned _iters = 1000);
 
    double norm1();//1范数   
    double norm2();//2范数   
    double mean();// 矩阵均值  
    double*operator[](int i){ return pmm + i*col; }//注意this加括号， (*this)[i][j]   
    void zeromean(_In_opt_  bool flag = true);//默认参数为true计算列
    void normalize(_In_opt_  bool flag = true);//默认参数为true计算列
    Matrix exponent(double x);//每个元素x次幂
    Matrix  eye();//对角阵
    void  maxlimit(double max,double set=0);//对角阵
};
 
/*
类方法的实现
*/
 
Matrix Matrix::mtanh() // 对应元素 tanh()
{
    Matrix ret(row, col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] = tanh(pmm[i]);
    }
    return ret;
}
double dets(int n, double *&aa)
{
    if (n == 1)
        return aa[0];
    double *bb = new double[(n - 1)*(n - 1)];//创建n-1阶的代数余子式阵bb        
    int mov = 0;//判断行是否移动       
    double sum = 0.0;//sum为行列式的值      
    for (int arow = 0; arow<n; arow++) // a的行数把矩阵a(nn)赋值到b(n-1)      
    {
        for (int brow = 0; brow<n - 1; brow++)//把aa阵第一列各元素的代数余子式存到bb      
        {
            mov = arow > brow ? 0 : 1; //bb中小于arow的行，同行赋值，等于的错过，大于的加一      
            for (int j = 0; j<n - 1; j++)  //从aa的第二列赋值到第n列      
            {
                bb[brow*(n - 1) + j] = aa[(brow + mov)*n + j + 1];
            }
        }
        int flag = (arow % 2 == 0 ? 1 : -1);//因为列数为0，所以行数是偶数时候，代数余子式为1.      
        sum += flag* aa[arow*n] * dets(n - 1, bb);//aa第一列各元素与其代数余子式积的和即为行列式    
    }
    delete[]bb;
    return sum;
}
 
Matrix Matrix::solveAb(Matrix &obj)
{
    Matrix ret(row, 1);
    if (size == 0 || obj.size == 0)
    {
        cout << "solveAb(Matrix &obj):this or obj is null" << endl;
        return ret;
    }
    if (row != obj.size)
    {
        cout << "solveAb(Matrix &obj):the row of two matrix is not equal!" << endl;
        return ret;
    }
 
    double *Dx = new double[row*row];
    for (int i = 0; i<row; i++)
    {
        for (int j = 0; j<row; j++)
        {
            Dx[i*row + j] = pmm[i*row + j];
        }
    }
    double D = dets(row, Dx);
    if (D == 0)
    {
        cout << "Cramer法则只能计算系数矩阵为满秩的矩阵" << endl;
        return  ret;
    }
 
 
    for (int j = 0; j<row; j++)
    {
        for (int i = 0; i<row; i++)
        {
            for (int j = 0; j<row; j++)
            {
                Dx[i*row + j] = pmm[i*row + j];
            }
        }
        for (int i = 0; i<row; i++)
        {
            Dx[i*row + j] = obj.pmm[i]; //obj赋值给第j列
        }
 
        //for( int i=0;i<row;i++) //print
        //{
        //    for(int j=0; j<row;j++)
        //    {
        //        cout<< Dx[i*row+j]<<"\t";
        //    }     
        //    cout<<endl;
        //}
        ret[j][0] = dets(row, Dx) / D;
         
    }
 
 
    delete[]Dx;
    return ret;
}
 
Matrix Matrix::getrow(size_t index)//返回行
{
    Matrix ret(1, col); //一行的返回值
 
    for (unsigned i = 0; i< col; i++)
    {
         
         ret[0][i] = pmm[(index) *col + i] ;
     
    }
    return ret;
}
 
Matrix Matrix::getcol(size_t index)//返回列
{
    Matrix ret(row, 1); //一列的返回值
 
 
    for (unsigned i = 0; i< row; i++)
    {
 
        ret[i][0] = pmm[i *col + index];
 
    }
    return ret;
}
 
Matrix Matrix::exponent(double x)//每个元素x次幂
{
    Matrix ret(row, col);
    double a;
    for (unsigned i = 0; i< row; i++)
    {
        for (unsigned j = 0; j < col; j++)
        {
            a=ret[i][j]= pow(pmm[i*col + j],x);
        }
    }
    return ret;
}
void Matrix::maxlimit(double max, double set)//每个元素x次幂
{
 
    for (unsigned i = 0; i< row; i++)
    {
        for (unsigned j = 0; j < col; j++)
        {
            pmm[i*col + j] = pmm[i*col + j]>max ? 0 : pmm[i*col + j];
        }
    }
  
}
Matrix Matrix::eye()//对角阵
{
 
    for (unsigned i = 0; i< row; i++)
    {
        for (unsigned j = 0; j < col; j++)
        {
            if (i == j)
            {
                pmm[i*col + j] = 1.0;
            }
        }
    }
    return *this;
}
void Matrix::zeromean(_In_opt_  bool flag)
{
    if (flag == true) //计算列均值
    {
        double *mean = new double[col];
        for (unsigned j = 0; j < col; j++)
        {
            mean[j] = 0.0;
            for (unsigned i = 0; i < row; i++)
            {
                mean[j] += pmm[i*col + j];
            }
            mean[j] /= row;
        }
        for (unsigned j = 0; j < col; j++)
        {
 
            for (unsigned i = 0; i < row; i++)
            {
                pmm[i*col + j] -= mean[j];
            }
        }
        delete[]mean;
    }
    else //计算行均值
    {
        double *mean = new double[row];
        for (unsigned i = 0; i< row; i++)
        {
            mean[i] = 0.0;
            for (unsigned j = 0; j < col; j++)
            {
                mean[i] += pmm[i*col + j];
            }
            mean[i] /= col;
        }
        for (unsigned i = 0; i < row; i++)
        {
            for (unsigned j = 0; j < col; j++)
            {
                pmm[i*col + j] -= mean[i];
            }
        }
        delete[]mean;
    }
}
 
void Matrix::normalize(_In_opt_  bool flag)
{
    if (flag == true) //计算列均值
    {
        double *mean = new double[col];
 
        for (unsigned j = 0; j < col; j++)
        {
            mean[j] = 0.0;
            for (unsigned i = 0; i < row; i++)
            {
                mean[j] += pmm[i*col + j];
            }
            mean[j] /= row;
        }
        for (unsigned j = 0; j < col; j++)
        {
 
            for (unsigned i = 0; i < row; i++)
            {
                pmm[i*col + j] -= mean[j];
            }
        }
        ///计算标准差
        for (unsigned j = 0; j < col; j++)
        {
            mean[j] = 0;
            for (unsigned i = 0; i < row; i++)
            {
                mean[j] += pow(pmm[i*col + j],2);//列平方和
            }
                mean[j] = sqrt(mean[j] / row); // 开方
        }
        for (unsigned j = 0; j < col; j++)
        {
            for (unsigned i = 0; i < row; i++)
            {
                pmm[i*col + j] /= mean[j];//列平方和
            }
        }
        delete[]mean;
    }
    else //计算行均值
    {
        double *mean = new double[row];
        for (unsigned i = 0; i< row; i++)
        {
            mean[i] = 0.0;
            for (unsigned j = 0; j < col; j++)
            {
                mean[i] += pmm[i*col + j];
            }
            mean[i] /= col;
        }
        for (unsigned i = 0; i < row; i++)
        {
            for (unsigned j = 0; j < col; j++)
            {
                pmm[i*col + j] -= mean[i];
            }
        }
        ///计算标准差
        for (unsigned i = 0; i< row; i++)
        {
            mean[i] = 0.0;
            for (unsigned j = 0; j < col; j++)
            {
                mean[i] += pow(pmm[i*col + j], 2);//列平方和
            }
            mean[i] = sqrt(mean[i] / col); // 开方
        }
        for (unsigned i = 0; i < row; i++)
        {
            for (unsigned j = 0; j < col; j++)
            {
                pmm[i*col + j] /= mean[i];
            }
        }
        delete[]mean;
    }
}
 
double Matrix::det()
{
    if (col == row)
        return dets(row, pmm);
    else
    {
        cout << ("行列不相等无法计算") << endl;
        return 0;
    }
}
/////////////////////////////////////////////////////////////////////      
istream &operator>>(istream &is, Matrix &obj)
{
    for (unsigned i = 0; i<obj.size; i++)
    {
        is >> obj.pmm[i];
    }
    return is;
}
 
ostream &operator<<(ostream &out, Matrix &obj)
{
    for (unsigned i = 0; i < obj.row; i++) //打印逆矩阵        
    {
        for (unsigned j = 0; j < obj.col; j++)
        {
            out << (obj[i][j]) << "\t";
        }
        out << endl;
    }
    return out;
}
ofstream &operator<<(ofstream &out, Matrix &obj)//打印逆矩阵到文件  
{
    for (unsigned i = 0; i < obj.row; i++)      
    {
        for (unsigned j = 0; j < obj.col; j++)
        {
            out << (obj[i][j]) << "\t";
        }
        out << endl;
    }
    return out;
}
Matrix operator+(const Matrix& lm, const Matrix& rm)
{
    if (lm.col != rm.col || lm.row != rm.row)
    {
        Matrix temp(0, 0);
        temp.pmm = NULL;
        cout << "operator+(): 矩阵shape 不合适,col:"
            << lm.col << "," << rm.col << ".  row:" << lm.row << ", " << rm.row << endl;
        return temp; //数据不合法时候，返回空矩阵      
    }
    Matrix ret(lm.row, lm.col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] = lm.pmm[i] + rm.pmm[i];
    }
    return ret;
}
Matrix operator-(const Matrix& lm, const Matrix& rm)
{
    if (lm.col != rm.col || lm.row != rm.row)
    {
        Matrix temp(0, 0);
        temp.pmm = NULL;
        cout << "operator-(): 矩阵shape 不合适,col:" 
            <<lm.col<<","<<rm.col<<".  row:"<< lm.row <<", "<< rm.row << endl;
 
        return temp; //数据不合法时候，返回空矩阵      
    }
    Matrix ret(lm.row, lm.col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] = lm.pmm[i] - rm.pmm[i];
    }
    return ret;
}
Matrix operator*(const Matrix& lm, const Matrix& rm)  //矩阵乘法
{
    if (lm.size == 0 || rm.size == 0 || lm.col != rm.row)
    {
        Matrix temp(0, 0);
        temp.pmm = NULL;
        cout << "operator*(): 矩阵shape 不合适,col:"
            << lm.col << "," << rm.col << ".  row:" << lm.row << ", " << rm.row << endl;
        return temp; //数据不合法时候，返回空矩阵      
    }
    Matrix ret(lm.row, rm.col);
    for (unsigned i = 0; i<lm.row; i++)
    {
        for (unsigned j = 0; j< rm.col; j++)
        {
            for (unsigned k = 0; k< lm.col; k++)//lm.col == rm.row      
            {
                ret.pmm[i*rm.col + j] += lm.pmm[i*lm.col + k] * rm.pmm[k*rm.col + j];
            }
        }
    }
    return ret;
}
Matrix operator*(double val, const Matrix& rm)  //矩阵乘 单数
{
    Matrix ret(rm.row, rm.col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] = val * rm.pmm[i];
    }
    return ret;
}
Matrix operator*(const Matrix&lm, double val)  //矩阵乘 单数
{
    Matrix ret(lm.row, lm.col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] = val * lm.pmm[i];
    }
    return ret;
}
 
Matrix operator/(const Matrix&lm, double val)  //矩阵除以 单数
{
    Matrix ret(lm.row, lm.col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] =  lm.pmm[i]/val;
    }
    return ret;
}
Matrix Matrix::multi(const Matrix&rm)// 对应元素相乘
{
    if (col != rm.col || row != rm.row)
    {
        Matrix temp(0, 0);
        temp.pmm = NULL;
        cout << "multi(const Matrix&rm): 矩阵shape 不合适,col:"
            << col << "," << rm.col << ".  row:" << row << ", " << rm.row << endl;
        return temp; //数据不合法时候，返回空矩阵      
    }
    Matrix ret(row,col);
    for (unsigned i = 0; i<ret.size; i++)
    {
        ret.pmm[i] = pmm[i] * rm.pmm[i];
    }
    return ret;
 
}
 
Matrix&  Matrix::operator=(const Matrix& rhs)
{
    if (this != &rhs)
    {
        row = rhs.row;
        col = rhs.col;
        size = rhs.size;
        if (pmm != NULL)
            delete[] pmm;
        pmm = new double[size];
        for (unsigned i = 0; i<size; i++)
        {
            pmm[i] = rhs.pmm[i];
        }
    }
    return *this;
}
//||matrix||_2  求A矩阵的2范数        
double Matrix::norm2()
{
    double norm = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        norm += pmm[i] * pmm[i];
    }
    return (double)sqrt(norm);
}
double Matrix::norm1()
{
    double sum = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        sum += abs(pmm[i]);
    }
    return sum;
}
double Matrix::mean()
{
    double sum = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        sum += (pmm[i]);
    }
    return sum/size;
}
 
 
 
 
void Matrix::sort(bool flag)
{
    double tem;
    for (unsigned i = 0; i<size; i++)
    {
        for (unsigned j = i + 1; j<size; j++)
        {
            if (flag == true)
            {
                if (pmm[i]>pmm[j])
                {
                    tem = pmm[i];
                    pmm[i] = pmm[j];
                    pmm[j] = tem;
                }
            }
            else
            {
                if (pmm[i]<pmm[j])
                {
                    tem = pmm[i];
                    pmm[i] = pmm[j];
                    pmm[j] = tem;
                }
            }
 
        }
    }
}
Matrix Matrix::diag()
{
    if (row != col)
    {
        Matrix m(0);
        cout << "diag():row != col" << endl;
        return m;
    }
    Matrix m(row);
    for (unsigned i = 0; i<row; i++)
    {
        m.pmm[i*row + i] = pmm[i*row + i];
    }
    return m;
}
Matrix Matrix::T()const
{
    Matrix tem(col, row);
    for (unsigned i = 0; i<row; i++)
    {
        for (unsigned j = 0; j<col; j++)
        {
            tem[j][i] = pmm[i*col + j];// (*this)[i][j]  
        }
    }
    return tem;
}
void  Matrix::QR(Matrix &Q, Matrix &R) const
{
    //如果A不是一个二维方阵，则提示错误，函数计算结束        
    if (row != col)
    {
        printf("ERROE: QR() parameter A is not a square matrix!\n");
        return;
    }
    const unsigned N = row;
    double *a = new double[N];
    double *b = new double[N];
 
    for (unsigned j = 0; j < N; ++j)  //(Gram-Schmidt) 正交化方法      
    {
        for (unsigned i = 0; i < N; ++i)  //第j列的数据存到a，b      
            a[i] = b[i] = pmm[i * N + j];
 
        for (unsigned i = 0; i<j; ++i)  //第j列之前的列      
        {
            R.pmm[i * N + j] = 0;  //      
            for (unsigned m = 0; m < N; ++m)
            {
                R.pmm[i * N + j] += a[m] * Q.pmm[m *N + i]; //R[i,j]值为Q第i列与A的j列的内积      
            }
            for (unsigned m = 0; m < N; ++m)
            {
                b[m] -= R.pmm[i * N + j] * Q.pmm[m * N + i]; //      
            }
        }
 
        double norm = 0;
        for (unsigned i = 0; i < N; ++i)
        {
            norm += b[i] * b[i];
        }
        norm = (double)sqrt(norm);
 
        R.pmm[j*N + j] = norm; //向量b[]的2范数存到R[j,j]      
 
        for (unsigned i = 0; i < N; ++i)
        {
            Q.pmm[i * N + j] = b[i] / norm; //Q 阵的第j列为单位化的b[]      
        }
    }
    delete[]a;
    delete[]b;
}
Matrix Matrix::eig_val(_In_opt_ unsigned _iters)
{
    if (size == 0 || row != col)
    {
        cout << "矩阵为空或者非方阵！" << endl;
        Matrix rets(0);
        return rets;
    }
    //if (det() == 0)  
    //{  
    //  cout << "非满秩矩阵没法用QR分解计算特征值！" << endl;  
    //  Matrix rets(0);  
    //  return rets;  
    //}  
    const unsigned N = row;
    Matrix matcopy(*this);//备份矩阵      
    Matrix Q(N), R(N);
    /*当迭代次数足够多时,A 趋于上三角矩阵，上三角矩阵的对角元就是A的全部特征值。*/
    for (unsigned k = 0; k < _iters; ++k)
    {
        //cout<<"this:\n"<<*this<<endl;      
        QR(Q, R);
        *this = R*Q;
        /*  cout<<"Q:\n"<<Q<<endl;
        cout<<"R:\n"<<R<<endl;  */
    }
    Matrix val = diag();
    *this = matcopy;//恢复原始矩阵；    
    return val;
}
Matrix Matrix::eig_vect(_In_opt_ unsigned _iters)
{
    if (size == 0 || row != col)
    {
        cout << "矩阵为空或者非方阵！" << endl;
        Matrix rets(0);
        return rets;
    }
    if (det() == 0)  
    {  
      cout << "非满秩矩阵没法用QR分解计算特征向量！" << endl;  
      Matrix rets(0);  
      return rets;  
    }  
    Matrix matcopy(*this);//备份矩阵     
    Matrix eigenValue = eig_val(_iters);
    Matrix ret(row);
    const unsigned NUM = col;
    double eValue;
    double sum, midSum, diag;
    Matrix copym(*this);
    for (unsigned count = 0; count < NUM; ++count)
    {
        //计算特征值为eValue，求解特征向量时的系数矩阵       
        *this = copym;
        eValue = eigenValue[count][count];
 
        for (unsigned i = 0; i < col; ++i)//A-lambda*I       
        {
            pmm[i * col + i] -= eValue;
        }
        //cout<<*this<<endl;      
        //将 this为阶梯型的上三角矩阵        
        for (unsigned i = 0; i < row - 1; ++i)
        {
            diag = pmm[i*col + i];  //提取对角元素    
            for (unsigned j = i; j < col; ++j)
            {
                pmm[i*col + j] /= diag; //【i,i】元素变为1    
            }
            for (unsigned j = i + 1; j<row; ++j)
            {
                diag = pmm[j *  col + i];
                for (unsigned q = i; q < col; ++q)//消去第i+1行的第i个元素    
                {
                    pmm[j*col + q] -= diag*pmm[i*col + q];
                }
            }
        }
        //cout<<*this<<endl;      
        //特征向量最后一行元素置为1    
        midSum = ret.pmm[(ret.row - 1) * ret.col + count] = 1;
        for (int m = row - 2; m >= 0; --m)
        {
            sum = 0;
            for (unsigned j = m + 1; j < col; ++j)
            {
                sum += pmm[m *  col + j] * ret.pmm[j * ret.col + count];
            }
            sum = -sum / pmm[m *  col + m];
            midSum += sum * sum;
            ret.pmm[m * ret.col + count] = sum;
        }
        midSum = sqrt(midSum);
        for (unsigned i = 0; i < ret.row; ++i)
        {
            ret.pmm[i * ret.col + count] /= midSum; //每次求出一个列向量      
        }
    }
    *this = matcopy;//恢复原始矩阵；    
    return ret;
}
Matrix Matrix::cov(bool flag)
{
    //row 样本数，column 变量数    
    if (col == 0)
    {
        Matrix m(0);
        return m;
    }
    double *mean = new double[col]; //均值向量    
 
    for (unsigned j = 0; j<col; j++) //init    
    {
        mean[j] = 0.0;
    }
    Matrix ret(col);
    for (unsigned j = 0; j<col; j++) //mean    
    {
        for (unsigned i = 0; i<row; i++)
        {
            mean[j] += pmm[i*col + j];
        }
        mean[j] /= row;
    }
    unsigned i, k, j;
    for (i = 0; i<col; i++) //第一个变量    
    {
        for (j = i; j<col; j++) //第二个变量    
        {
            for (k = 0; k<row; k++) //计算    
            {
                ret[i][j] += (pmm[k*col + i] - mean[i])*(pmm[k*col + j] - mean[j]);
 
            }
            if (flag == true)
            {
                ret[i][j] /= (row-1);
            }
            else
            {
                ret[i][j] /= (row);
            }
            /*temp.Format("cov=%f,column=%d mean(%d)=%f,mean(%d)=%f",cov[i*column+j],row,i,mean[i],j,mean[j]);
            MessageBox(temp);*/
        }
    }
    for (i = 0; i<col; i++) //补全对应面    
    {
        for (j = 0; j<i; j++)
        {
            ret[i][j] = ret[j][i];
        }
    }
    return ret;
}
Matrix Matrix::adjoint()
{
    //调动之前，检查时候方阵，这里默认为aa为方阵    
    //确定本函数是否修改传入的数据 ：no    
    //调用函数内删除内存delete []padjoint;    
    if (row != col)
    {
        Matrix ret(0);
        return ret;
    }
 
    const int n = row;
    Matrix ret(row);
    double *bb = new double[(n - 1)*(n - 1)];//创建n-1阶的代数余子式阵bb     
 
    int pi, pj, q;
    for (int ai = 0; ai<n; ai++) // a的行数把矩阵a(nn)赋值到b(n-1)    
    {
        for (int aj = 0; aj<n; aj++)
        {
            for (int bi = 0; bi<n - 1; bi++)//把元素aa[ai][0]余子式存到bb[][]    
            {
                for (int bj = 0; bj<n - 1; bj++)//把元素aa[ai][0]代数余子式存到bb[][]    
                {
                    if (ai>bi)    //ai行的代数余子式是：小于ai的行，aa与bb阵，同行赋值    
                        pi = 0;
                    else
                        pi = 1;     //大于等于ai的行，取aa阵的ai+1行赋值给阵bb的bi行    
                    if (aj>bj)    //ai行的代数余子式是：小于ai的行，aa与bb阵，同行赋值    
                        pj = 0;
                    else
                        pj = 1;     //大于等于ai的行，取aa阵的ai+1行赋值给阵bb的bi行    
                    bb[bi*(n - 1) + bj] = pmm[(bi + pi)*n + bj + pj];
                }
            }
            if ((ai + aj) % 2 == 0)  q = 1;//因为列数为0，所以行数是偶数时候，代数余子式为-1.    
            else  q = (-1);
            ret.pmm[ai*n + aj] = q*dets(n - 1, bb);  //加符号变为代数余子式    
        }
    }
    delete[]bb;
    return ret;
}
Matrix Matrix::inverse()
{
    double det_aa = det();
    if (det_aa == 0)
    {
        cout << "行列式为0 ，不能计算逆矩阵。" << endl;
        Matrix rets(0);
        return rets;
    }
    Matrix adj = adjoint();
    Matrix ret(row);
 
    for (unsigned i = 0; i<row; i++) //print    
    {
        for (unsigned j = 0; j<col; j++)
        {
            ret.pmm[i*col + j] = adj.pmm[i*col + j] / det_aa;
        }
    }
    return ret;
}


