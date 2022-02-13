#include <bits/stdc++.h>

using namespace std;

typedef struct time_shift {
    vector<int> week;
    int day;
    vector<int> stt;
} t_s;

typedef struct a_school_shift {
    string instructor;
    string room;
    t_s t;
} shift;

typedef struct grade {
    vector<shift> list_shift;
} grade;

typedef struct component_class {
    string type_class;
    vector<vector<grade>> cc;
} component_class;

typedef struct subjects {
    string code;
    string name;
    int number_credits;
    vector<component_class> list_comp;
} subjects;

vector<string> tokenize(string const &str, char const delim) {
    vector<string> out;
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != string::npos) {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
    return out;
}

void check_day(string str, int &day) {
    vector<string> days = {
        "M",
        "T",
        "W",
        "Th",
        "F",
        "Sat",
        "Sun"};

    for (int i = 0; i < days.size(); ++i) {
        if (str == days[i]) {
            day = i + 1;
            break;
        }
    }
}

void display_vector(vector<int> v) {
    for (auto i : v) {
        cout << i + 1 << " ";
    }
    cout << '\n';
}

void display(vector<subjects> list_subj) {
    int i = 1, j;
    for (auto sub : list_subj) {
        cout << i++ << "\t" << sub.code << "\n";
        for (auto comp : sub.list_comp) {
            cout << "\t---------------------------------";
            cout << "---------------------------------\n";
            cout << "\t|" + comp.type_class + "\tTuan\tThu\tTiet\tPhong\tGiang vien\t\n";
            j = 0;
            for (auto session : comp.cc) {
                j++;
                for (auto gr : session) {
                    // cout << "\t\t" << gr.list_shift[0].t.week.size();
                    cout << "\t|" << j << "\t|";

                    cout << gr.list_shift[0].t.stt[0] << "->" << gr.list_shift[0].t.stt[1] << "\t" << gr.list_shift[0].t.day + 1 << "\t";
                    cout << gr.list_shift[0].t.week[0] << "-" << gr.list_shift[0].t.week[1] << "\t";
                    cout << gr.list_shift[0].room << "\t" << gr.list_shift[0].instructor << "\n";
                }
            }
        }
        cout << "==========================================";
        cout << "==========================================\n";
    }
}

vector<subjects> read_data() {
    vector<subjects> list;
    string tmp;
    int i, index_subj, index_type, index_session, index_grade;

    while (getline(cin, tmp)) {
        vector<string> token = tokenize(tmp, ',');
        // cout << tmp + "\n";

        // find index subject
        index_subj = -1;
        for (i = 0; i < list.size(); ++i) {
            if (list[i].code == token[0]) {
                index_subj = i;
                break;
            }
        }
        if (index_subj == -1) {
            index_subj = list.size();
            list.push_back({});
            list[index_subj].code = token[0];
        }

        // find index type
        index_type = -1;
        for (i = 0; i < list[index_subj].list_comp.size(); ++i) {
            if (list[index_subj].list_comp[i].type_class == token[1]) {
                index_type = i;
                break;
            }
        }
        if (index_type == -1) {
            index_type = list[index_subj].list_comp.size();
            list[index_subj].list_comp.push_back({});
            list[index_subj].list_comp[index_type].type_class = token[1];
        }

        // value index_session
        index_session = stoi(token[2]) - 1;
        while (index_session >= list[index_subj].list_comp[index_type].cc.size()) {
            list[index_subj].list_comp[index_type].cc.push_back({});
        }

        // index_grade
        index_grade = list[index_subj].list_comp[index_type].cc[index_session].size();
        list[index_subj].list_comp[index_type].cc[index_session].push_back({});

        shift sh;
        sh.t.stt = {stoi(tokenize(token[3], '-')[0]), stoi(tokenize(token[3], '-')[1])};
        check_day(token[4], sh.t.day);
        sh.t.week = {stoi(token[5]), stoi(token[6])};
        sh.room = token[7];
        sh.instructor = token[8];

        list[index_subj].list_comp[index_type].cc[index_session][index_grade].list_shift.push_back(sh);
    }
    // display(list);

    return list;
}

bool check_intersect(vector<int> &d1, vector<int> &d2) {
    if ((d1[0] - d2[0]) * (d1[0] - d2[1]) <= 0)
        return true;
    if ((d1[1] - d2[0]) * (d1[1] - d2[1]) <= 0)
        return true;
    if ((d1[0] - d2[0]) * (d1[1] - d2[0]) <= 0)
        return true;
    return false;
}

bool check_time(vector<t_s> &t, t_s &t_detail) {
    for (auto i : t) {
        if (i.day == t_detail.day && check_intersect(i.stt, t_detail.stt) && check_intersect(i.week, t_detail.week))
            return false;
    }
    return true;
}

bool check_subject(subjects &s, int &ind_sess, vector<int> &index_grade, int &start_i_comp, vector<t_s> &ts) {
    // check lịch môn học (s) với các môn trc đấy
    for (int j = start_i_comp; j < index_grade.size(); ++j) {
        if (!check_time(ts, s.list_comp[j].cc[ind_sess][index_grade[j]].list_shift[0].t))
            return false;
    }
    return true;
}

int next_index(subjects &s, int &ind_sess, vector<int> &current_index, int distance) {
    // VD : 101 + 1 = 110 với mỗi thành phần trong sess đều có 2 lớp để đăng ký

    int tmp = current_index.size() - 1;
    while (current_index[tmp] + distance >= s.list_comp[tmp].cc[ind_sess].size()) {
        current_index[tmp] = 0;
        --tmp;
        if (tmp < 0)
            return -1;  // khi không thể chuyển đến tổ hợp lớp tiếp theo của môn học trong session thì trả về -1
    }
    current_index[tmp] += distance;

    return tmp;  // trả về vị trí thành phần môn học thay đổi lớp học
}

bool solve(vector<subjects> &subj, vector<vector<string>> &sol, vector<t_s> &ts, int &index_subj) {
    cout << "-------------------------------subject " << index_subj + 1 << "\n";
    if (index_subj >= subj.size())
        return true;
    sol.push_back({});
    sol[index_subj].push_back(subj[index_subj].code);

    int i, j, ind_sess, vt_;
    subjects s_tmp = subj[index_subj];
    vector<component_class> list_comp = s_tmp.list_comp;
    int n_sess = list_comp[0].cc.size();
    vector<int> index_cur;

    for (i = 0; i < list_comp.size(); ++i)
        index_cur.push_back(0);

    // cout << "subject = " << index_subj + 1 << "; number session =  " << n_sess << "\n";

    for (ind_sess = 0; ind_sess < n_sess; ++ind_sess) {
        // in ra để xem quá trình quay lui
        // cout << index_subj + 1 << "  --->  index session = " << ind_sess + 1 << "\n";

        sol[index_subj].push_back(" Sess: " + to_string(ind_sess + 1) + "\n");

        vt_ = 0;
        while (vt_ != -1) {
            // in ra để xem qua trinh quay lui
            cout << "sub_" << index_subj + 1 << " = ";
            display_vector(index_cur);

            if (check_subject(s_tmp, ind_sess, index_cur, vt_, ts)) {
                // them thoi gian hoc vao ts
                for (j = vt_; j < index_cur.size(); ++j) {
                    t_s t = list_comp[j].cc[ind_sess][index_cur[j]].list_shift[0].t;
                    ts.push_back(t);
                }

                for (j = 0; j < index_cur.size(); ++j) {
                    string str = list_comp[j].type_class;
                    t_s t = list_comp[j].cc[ind_sess][index_cur[j]].list_shift[0].t;
                    str += "\tThu:" + to_string(t.day + 1);
                    str += "\tTiet:" + to_string(t.week[0]) + " -> " + to_string(t.week[1]);
                    str += "\tTuan:" + to_string(t.stt[0]) + " -> " + to_string(t.stt[1]);
                    str += "\tRoom: " + list_comp[j].cc[ind_sess][index_cur[j]].list_shift[0].room;
                    str += "\t" + list_comp[j].cc[ind_sess][index_cur[j]].list_shift[0].instructor;
                    str += "\n";
                    sol[index_subj].push_back(str);
                }

                ++index_subj;
                if (solve(subj, sol, ts, index_subj)) {
                    // in ra kết quả
                    cout << "session " << ind_sess + 1 << ":  ";
                    display_vector(index_cur);

                    return true;
                }
                --index_subj;

                for (j = vt_; j < index_cur.size(); ++j) {
                    ts.pop_back();
                }

                for (j = 0; j < index_cur.size(); ++j) {
                    sol[index_subj].pop_back();
                }
            }

            vt_ = next_index(s_tmp, ind_sess, index_cur, 1);  // vi tri thay doi chi so
        }
        sol[index_subj].pop_back();
    }
    sol.pop_back();
    return false;
}

int main(int argc, char const *argv[]) {
    freopen("tkb_input.csv", "r", stdin);
    freopen("tkb_output", "w", stdout);
    vector<vector<string>> sol;
    vector<t_s> ts;
    int index = 0;
    vector<subjects> subj = read_data();
    // display(subj);

    if (solve(subj, sol, ts, index)) {
        for (auto i : sol) {
            cout << "\n------------------------------------\n";
            for (auto j : i) {
                cout << j << " \t";
            }
        }
        cout << '\n';
    } else {
        cout << "Failed! \n";
    }
    return 0;
}