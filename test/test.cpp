#include <iostream>
#include <boost/circular_buffer.hpp>
using namespace std;

void func(boost::circular_buffer<int>& cb){
    cout<<"------------------------------"<<endl;
    for(boost::circular_buffer< int >::reverse_iterator i = cb.rbegin(); i != cb.rend() ; i++){
        cout<<*i<<endl;
    }
}
int main(int argc, char** argv){
    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    cout<<(i3 & 1)<<endl;
    cout<<(i3 & 2)<<endl;
    return 0;
    boost::circular_buffer<int> cb(3);
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);
    cb.push_back(10);
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);
    cb.push_back(11);
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);
    cb.push_back(12);
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);
    cb.push_back(13);
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);
    //cb.clear();
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);
    cb.push_back(14);
    cout<<"size:"<<cb.size() <<", back:"<<cb.back()<<endl;
    func(cb);

    return 0;
}
