int foobar (const std::vector<std::string> &a, int b) { //O(a)
    int answer = 0;
    for (int i = 0; i < a.size(); i+=b) {
        answer++;
    }
    return answer;
}

void foo2 (const std::vector<int> &a, std::string &b) { //O(a)
    b.clear();
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] > 0)
            b.push_back('+');
        else
            b.push_back('-');
    }
}

std::vector<int> foo3 (const std::vector<int> &a, const std::string &b) { //O(n)
    return std::vector<int>(b.size(),a.size()); //Technically O(2)
}

int foo3 (const std::vector<std::string> &a, const std::string& b) { //O(a)
    int ret = 0;
    for (int i=0; i<a.size(); i++){
        ret += (a[i] == b);
    }
    return ret;
}

std::vector<int> foo4 (const std::vector<int> &a) { //O(ae)
    std::vector<int> answer = a;
    for (int i = 0; i < a.size(); i++) {//a
        if(a[i] < (a[a.size()-1]*a[a.size()-1])){
            answer.erase(answer.end()-1);//e
        }
    }
    return answer;
}

std::vector<int> foo5 (const std::vector<int> &a, int b) { //O(ai)
    std::vector<int> ret;
    for(int i=0; i<a.size(); i++){//a
        if(a[i] < b){
            ret.insert(ret.end(),a[i]);//i, but functions as a push_back...O(1)?
        }
    }
    return ret;
}
//====================================================================================
int foobar (const std::vector<std::string> &a, int b) {//O(a/2)=O(a)
    int answer = 0;
    for (int i = 0; i < a.size(); i+=b) {
        answer++;
    }
    return answer;
}

std::vector<int> bar2 (const std::vector<std::string> &a) {//O(a)
    std::vector<int> answer;
    for (int i = 0; i < a.size(); i++) {
        answer.push_back(a[i].size());
    }
    return answer;
}

std::vector<std::string> bar3 (const std::vector<int> &a) {//O(ap)?
    std::vector<std::string> answer;
    for (int i = 0; i < a.size(); i++) {//a
        answer.push_back(std::string(a[i],'+'));//string = p
    }
    return answer;
}

void bar3 (std::vector<std::string> &a, const std::string &b) { //O(a)
    for (int i = 0; i < a.size(); i++) {
        a[i] = b;
    }
}

std::vector<int> bar4 (const std::vector<std::string> &a) {//O(an)
    std::vector<int> answer;
    if(!a.empty()){
        for (int i = 0; i < std::min(a[0].size(), a.size()); i++) {
            answer.insert(answer.begin(),a[i].size());
        }
    }
    return answer;
}

void bar5 (std::vector<int> &a) { //O(a)
    for (int i = 0; i < a.size(); i++){
        if (a[i] > 0){
            a.erase(a.begin() + i);
            i--;
        }
    }
}