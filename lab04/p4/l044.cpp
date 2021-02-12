// Anup Bagali
// Period 7
// 02/12/2021

#include <bits/stdc++.h>

#define KD 2
#define K 5


using namespace std;

template<class T, int N>
class Comparable{
    public:
        array<T, N> data;
        Comparable(const T(&list)[N]){
            for(int i=0;i<N;i++){
                this->data[i] = list[i];
            }
        };
        void print(){
            for(int i=0;i<N;i++)
                printf("%lf ", this->data[i]);
            printf("\n");
        }
        inline double distance(Comparable<T, N>& c){
            double d = 0;
            for(int i=0;i<N;i++)
                d += pow(this->data[i-c.data[i]], 2);
            return sqrt(d);
        }
        bool operator==(Comparable<T, N>& c){
            return this->data == c.data;
        }
        bool operator!=(Comparable<T, N>& c){
            return this->data != c.data;
        }
};


class Point: public Comparable<double, 2>{
    public:
        Point(const double(&list)[2]): Comparable(list) {};
        Point(): Comparable<double, 2>({rand()/(double)RAND_MAX, rand()/(double)RAND_MAX}) {};
        Point(Point& p1, double scalar): Comparable({p1.data[0]*scalar, p1.data[1]*scalar}) {};
        Point(Point& p1, double s_x, double s_y): Comparable({p1.data[0]*s_x, p1.data[1]*s_y}) {};
};


template<typename T>
class Node{
    private:
        Node *_l, *_r;
        T _value;
    public:
        Node(T val): l(NULL), R(NULL), _value(val) {};

        Node<T>* get_left(){
            return this->_l;
        }

        Node<T>* get_right(){
            return this->_r;
        }

        T get_value(){
            return this->_value;
        }

        void set_left(Node<T>* n){
            this->_l = n;
        }

        void set_right(Node<T>* n){
            this->_r = n;
        }

        void set_value(T val){
            this->_value = val;
        }
};


int main(int argc, char* argv[]){
}
