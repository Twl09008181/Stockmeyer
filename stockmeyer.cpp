#include <vector>
#include <utility>
#include <stack>
#include <iostream>
#include <algorithm>

using shape = std::pair<int,int>;  // w x h

bool compareW(const shape&s1,const shape&s2){return s1.first <= s2.first;}
bool compareH(const shape&s1,const shape&s2){return s1.second <= s2.second;}

std::vector<shape> getPacking(std::vector<int>&PolishExpression,std::vector<std::vector<shape>>&shapes);
std::vector<shape> stockmeyer(std::vector<shape>&shape1,std::vector<shape>&shape2,bool H_cut);


void showShape(std::vector<shape>&shapes)
{
    for(auto &s:shapes){std::cout<<"("<<s.first<<","<<s.second<<")  ";}
    std::cout<<"\n";
}
int main()
{


    //example 1

    //1 2 H 3 4 V 5 6 V H V 
    // std::vector<int>PolishExp = {1,2,-1,3,4,-2,5,6,-2,-1,-2};

    // //shape
    // std::vector<std::vector<shape>>Shapes = {
    //     std::vector<shape>{}, //empty
    //     std::vector<shape>{{3,2},{2,3}}, //block1 
    //     std::vector<shape>{{2,2}},       //block2
    //     std::vector<shape>{{3,1},{1,3}},
    //     std::vector<shape>{{3,2},{2,3}},
    //     std::vector<shape>{{2,1},{1,2}},
    //     std::vector<shape>{{2,2}},
    // };

    // std::vector<shape> packing = getPacking(PolishExp,Shapes);

    // showShape(packing);


    //homework 4
    //1 2 V 3 V 4 H
    std::vector<int>PolishExp = {1,2,-2,3,-2,4,-1};

    //shape
    std::vector<std::vector<shape>>Shapes = {
        std::vector<shape>{}, //empty
        std::vector<shape>{{3,2},{2,3}}, //block1 
        std::vector<shape>{{2,4},{4,2}},       //block2
        std::vector<shape>{{5,5}},
        std::vector<shape>{{6,3}}
    };

    std::vector<shape> packing = getPacking(PolishExp,Shapes);

    showShape(packing);


    return 0;
}
std::vector<shape> getPacking(std::vector<int>&PolishExpression,std::vector<std::vector<shape>>&shapes){


    std::stack<std::vector<shape>> Stack;

    for(auto token:PolishExpression)
    {
        if(token < 0){
            auto s1 = Stack.top();Stack.pop();
            auto s2 = Stack.top();Stack.pop();
            if(token==-1){//Hcut
                auto result = stockmeyer(s1,s2,true);
                Stack.push(result);
                showShape(result);
            }else if(token==-2)
            {
                auto result = stockmeyer(s1,s2,false);
                Stack.push(result);
                showShape(result);
            }
            else{
                std::cerr<<"error PolishExpression\n";
            }
        }
        else{
            Stack.push(shapes.at(token));
        }
    }
    return Stack.top();
}

std::vector<shape> stockmeyer(std::vector<shape>&shape1,std::vector<shape>&shape2,bool H_Cut){


    decltype(compareH)* cutway = (H_Cut) ? compareH: compareW; //如果是水平cut, 就把高度由小排到大 , 這樣寬度就會由大到小排好
    std::sort(shape1.begin(),shape1.end(),cutway);
    std::sort(shape2.begin(),shape2.end(),cutway);
    // H cut join(b1,b2) = (max(b1.w,b2.w),b1.h + b2.h) 


    std::vector<shape> combine;
    int i = 0,j = 0;
    while(i < shape1.size() && j < shape2.size()){
        int w1 = shape1.at(i).first;
        int h1 = shape1.at(i).second;            
        int w2 = shape2.at(j).first;
        int h2 = shape2.at(j).second;

        if(H_Cut){
            combine.push_back({std::max(w1,w2),h1+h2});
            if(w1 > w2)i++;
            else if(w1 < w2)j++;
            else {i++;j++;}
        }
        else{
            combine.push_back({w1+w2,std::max(h1,h2)});
            if(h1 > h2)i++;
            else if(h1 < h2)j++;      
            else{i++;j++;}
        }
    }
    return combine;        
}
