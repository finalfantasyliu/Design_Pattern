#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

struct Journal {
    string title;
    vector<string> entries;

    explicit Journal(const string &title) : title(title) {
    }

    void add(const string &entry) {
        static int count = 1;
        entries.push_back(to_string(count++) + ": " + entry);
    }
    //這個save功能就算persistence of concerns，因為可能有其他class也有需要輸出的功能
    //void save(const string &fileName)
};

//persistence of concerns
//之後可以在 persistenceManager這個class 創造各種輸出的方式，
//因此不需要更改任何需要使用輸出功能的class
struct PersistenceManager {
    static void save(const Journal &j, const string &fileName) {
        ofstream ofs(fileName);
        if (ofs.is_open())
            for (auto &entry: j.entries)
                ofs << entry << endl;
        else
            cerr<<"Can't open file"<<endl;
        if (ofs.is_open())
            ofs.close();
    }
};

int main() {
    Journal journal{"Diary"};
    journal.add("terrible company");
    journal.add("job interview");
    journal.add("got offer");
    PersistenceManager persistenceManager;
    persistenceManager.save(journal,"diary.txt");
    return 0;
}
