#include "ContextualSummaries.h"

void consume_queue(std::queue<std::string>& q){
    while(!q.empty()){
        std::cout << q.front() << ' ';
        q.pop();
    }
}

void add_to_5queue(std::queue<std::string>& q,const std::string & word){
    if(q.size() == 5){
        q.pop();
    }
    q.push(word);
}

void write_context(std::list<std::pair<int,int>> posSpan,const std::string & filename){
    posSpan.sort();
    std::ifstream f;
    f.open(filename);
    if(f.is_open()){
        int counter = 0; //current word position
        int toHighlight = 0; //future words to highlight if span > 1
        int toPrint = 0; //future words to print after highlighted words
        int sincePrint = 0; //used to know when to put "..." before consuming queue
        bool sincePrintFlag = false; //used with counter above^
        std::list<std::pair<int,int>>::iterator it = posSpan.begin(); //iterator to step through positions to highlight
        std::queue<std::string> q; //queue to store up to last 5 words to prefix
        std::string line, word;
        while(std::getline(f,line)){
            std::istringstream ss(line);
            while(std::getline(ss,word,' ')){
                if(counter == it->first){ //at word position to highlight
                    //print out prefix wrapping words and "..." if needed
                    if(sincePrintFlag){
                        if(sincePrint > 5){
                            std::cout << "...";
                        } else {
                            std::cout << ' ';
                        }
                    }
                    consume_queue(q);
                    //highlight word, and setup for spans > 1
                    if(it->second == 1){
                        std::cout << "<b>" << word << "</b> ";
                    } else {
                        std::cout << "<b>" << word << ' ';
                        toHighlight = it->second - 1;
                    }
                    //setup postfix words to print
                    toPrint = 5;
                    sincePrint = 0;
                    sincePrintFlag = false;
                    it++;
                } else if(toHighlight > 0){ //span was > 1, keep up bolding
                    if(toHighlight == 1){
                        std::cout << word << "</b> ";
                    } else {
                        std::cout << word << ' ';
                    }
                    toHighlight--;
                } else if(toPrint > 0){ //words have been highlighted, onto postfix words to print
                    if(toPrint == 1 && (it->first >= counter + 6 || it == posSpan.end())){
                        std::cout << word;
                        sincePrintFlag = true;
                    } else {
                        std::cout << word << ' ';
                    }
                    toPrint--;
                } else { //just consuming words
                    if(word.back() == '.'){
                        while(!q.empty()){q.pop();} //confine context to current sentance
                    } else {
                        add_to_5queue(q,word);
                    }
                    if(sincePrintFlag){sincePrint++;}
                }
                counter++;
            }
        }
        if(sincePrint > 0){std::cout << "...";} //final "..." if required
    } else {
        throw std::runtime_error ("Error openning text file.");
    }
    f.close();
}

std::list<std::pair<int,int>> get_word_span_list(const std::string & filename){
    std::ifstream f;
    std::list<std::pair<int,int>> posSpan;
    f.open(filename);
    if(f.is_open()){
        std::string line;
        while(std::getline(f,line)){
            std::string num1s, num2s;
            bool first = true;
            for(std::string::iterator it = line.begin(); it != line.end(); ++it){
                char c = *it;
                if(c == ','){
                    first = false;
                }
                else if(first){
                    num1s += c;
                }
                else {
                    num2s += c;
                }
            }
            int num1, num2;
            num1 = std::stoi(num1s); num2 = std::stoi(num2s);
            posSpan.push_back(std::make_pair (num1,num2));
        }
    } else {
        throw std::runtime_error ("Error openning word positions file.");
    }
    f.close();
    return posSpan;
}

int main(int argc, char** argv){
    //read word positions and span from file and construct my list
    if(argc != 3){
        std::cerr << "Please enter two arguements only." << std::endl;
        std::cerr << "The first should be a list of pairs of ints." << std::endl;
        std::cerr << "The second should be the filename of the text." << std::endl;
        return 1;
    }
    std::list<std::pair<int,int>> posSpan;
    try{
        posSpan = get_word_span_list(argv[1]);
    } catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return 1;
    }
    //call write context with list and correct text file
    try{
        write_context(posSpan,argv[2]);
    } catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        return 1;
    }
    std::cout << std::endl;
    return 0;
}