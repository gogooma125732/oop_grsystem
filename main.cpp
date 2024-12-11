#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip> // for setw()
#include <algorithm>
#include <cwchar>
#include <locale>
#include <wchar.h>
using namespace std;
/*
        ###  ##  ##   ##  ###
        ###  ##  ### ###  ###
        ###  ##  #######  ###
        ###  ##  ## # ##  ###
         #####   ##   ##  #######

 */

 //UserInfo for Login
struct UserInfo { //public
    int index = 0;
    string name = "";
    int id = 0;
    string password = "";
    int userType = 0; // 1: 학생, 2: 교수, 3: 관리자

    UserInfo(int idx, const string& userName, int userId, const string& userPassword, int type)
        : index(idx), name(userName), id(userId), password(userPassword), userType(type) {}
};

vector<UserInfo> loadUserInfo(const string& filename) {
    vector<UserInfo> users;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return users;
    }

    string line;
    getline(file, line); // 헤더 줄 건너뜀
    while (getline(file, line)) {
        stringstream ss(line);
        string temp;

        // 데이터를 순차적으로 읽고 구조체에 저장
        getline(ss, temp, '\t');  // Index
        int index = stoi(temp);

        getline(ss, temp, '\t');  // Name
        string name = temp;

        getline(ss, temp, '\t');  // Id
        int id = stoi(temp);

        getline(ss, temp, '\t');  // pw
        string password = temp;

        getline(ss, temp, '\t');  // userType
        int userType = stoi(temp);

        users.push_back(UserInfo(index, name, id, password, userType));
    }
    file.close();
    return users;
}

bool login(const vector<UserInfo>& users, int userType, int id, const string& password) {
    for (const auto& user : users) {
        if (user.userType == userType && user.id == id && user.password == password) {
            return true;
        }
    }
}
//

// Subject 클래스
class Subject {
protected:
    string subjectName, professor, student;
    int semester, credit;
    string gradingSystem;
    string grade;

public:
    Subject(const string& name, const string& prof, const string& stu,
        int sem, int cred, const string& grading, const string& gr)
        : subjectName(name), professor(prof), student(stu), semester(sem), credit(cred), gradingSystem(grading), grade(gr) {} //--
    Subject(int sem, const string& name, int cred, const string& gr)
        : semester(sem), subjectName(name), credit(cred), grade(gr) {} //Stu
    Subject(int sem, const string& name, const string& stu, const string& gr)
        : semester(sem), subjectName(name), student(stu), grade(gr) {} //Pro
    Subject(int sem, const string& name, const string& pro, const string& stu, const string& gr)
        : semester(sem), subjectName(name), professor(pro), student(stu), grade(gr) {} //All

    virtual void display() const = 0;

    string getStudent() const { return student; }
    string getProfessor() const { return professor; }
};
// 년도, 학기, 과목명, 학점, 성적
// 학생 과목
class StudentSubject : public Subject {
public:
    StudentSubject(int sem, const string& name, int cred, const string& gr)
        : Subject(sem, name, cred, gr) {}

    void display() const override {
        cout << semester << "   |" << subjectName << "|   " << credit << "   |   " << grade << endl;
    }
};
// 년도/학기, 과목명, 학생, 성적
// 교수 과목
class ProfessorSubject : public Subject {
public:
    ProfessorSubject(int sem, const string& name, const string& stu, const string& gr)
        : Subject(sem, name, stu, gr) {}

    void display() const override {
        cout << semester << "   |" << subjectName << "|   " << student << "   |   " << grade << endl;
    }
};
// 년도/학기, 과목명, 교수, 학생, 학점, 성적
// 관리자 과목
class AllSubject : public Subject {
public:
    AllSubject(int sem, const string& name, const string& pro, const string& stu, const string& gr)
        : Subject(sem, name, pro, stu, gr) {}

    void display() const override {
        cout << semester << "   |" << subjectName << "|   " << professor << "   |   " << student << "   |   " << grade << endl;
    }
};

// 파일에서 데이터를 읽어오기
vector<vector<string>> loadData(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filename << endl;
        return data;
    }

    string line;

    // 첫 번째 줄(헤더)을 건너뛰기
    getline(file, line);
    // 두 번째 줄부터 읽음
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> values;
        string value;
        while (getline(ss, value, ',')) {
            values.push_back(value);
        }
        data.push_back(values);
    }
    file.close();
    return data;
}


// 안전한 stoi 함수 (예외 처리)
int safe_stoi(const string& str) {
    try {
        return stoi(str);
    }
    catch (const out_of_range&) {
        cerr << "범위를 벗어난 값: " << str << endl;
        return -1;
    }
    catch (const invalid_argument&) {
        cerr << "변환할 수 없는 값: " << str << endl;
        return -1;  // 오류 발생 시 -1 반환 (적절한 에러 처리 필요)
    }
}
// 년도/학기, 과목명, 학점, 성적
// 수정된 displayStudentSubjects 함수
void displayStudentSubjects(const vector<vector<string>>& data, const int& id) {
    for (const auto& row : data) {
        if (safe_stoi(row[5]) == id) {  // 학생 ID와 일치하는 경우 출력
            StudentSubject subject(safe_stoi(row[6]), row[1], safe_stoi(row[8]), row[14]);
            subject.display();
        }
    }
}
// 년도/학기, 과목명, 학생, 성적
// 수정된 displayProfessorSubjects 함수
void displayProfessorSubjects(const vector<vector<string>>& data, const int& id) {
    for (const auto& row : data) {
        if (safe_stoi(row[4]) == id) {  // 교수 ID와 일치하는 경우 출력
            ProfessorSubject subject(safe_stoi(row[6]), row[1], row[3], row[14]);
            subject.display();
        }
    }
}
// 년도/학기, 과목명, 교수, 학생, 학점, 성적
// 관리자 모든 정보 출력
void displayAllSubjects(const vector<vector<string>>& data) {
    for (const auto& row : data) {
        AllSubject subject(safe_stoi(row[6]), row[1], row[2], row[3], row[14]);
        subject.display();
    }
}




// 교수 성적 수정
// 
// 점수 업데이트 함수
void displaySubjects(const string& filename, const int& professorId, const vector<vector<string>>& data, const vector<string>& columnNames) {
    cout << "수정 가능한 과목 학생 목록:" << endl;
    for (int i = 1; i < data.size(); ++i) {  // 데이터의 첫 번째 행은 컬럼명
        int subjectProfessorId = safe_stoi(data[i][4]);  // 교수 ID가 있는 열을 확인 (4번째 열로 가정)

        if (subjectProfessorId == professorId) {
            cout << i << ". 과목명: " << data[i][1] << ", 학기: " << data[i][6] << ", 학생: " << data[i][3] << ", 학생ID: " << data[i][5] << endl;
        }
    }
}

void reviseProfessorSubjects(const string& filename, const int& professorId, vector<vector<string>>& data, const vector<string>& columnNames) {
    // 수정가능한 과목 보여주기
    displaySubjects(filename, professorId, data, columnNames);

    bool found = false;
    int studentId;

    // 수정할 학생 ID 입력받기
    cout << "수정할 학생 ID를 입력하세요: ";
    cin >> studentId;

    // 교수, 과목, 학생ID에 해당하는 성적 조회; 학생 ID와 이 일치하는 데이터 찾기
    for (auto& row : data) {
        if (safe_stoi(row[5]) == studentId && safe_stoi(row[4]) == professorId) {  // 교수 ID와 일치하는 경우 출력
            found = true;

            // 학생 정보 출력
            cout << "학생명: " << row[3] << ", 학생 ID: " << row[5] << endl;

            // 성적 수정 요청
            cout << "중간 점수를 입력하세요 (현재: " << row[9] << "): ";
            string midtermScore;
            cin >> midtermScore;

            cout << "기말 점수를 입력하세요 (현재: " << row[10] << "): ";
            string finalScore;
            cin >> finalScore;

            cout << "과제 점수를 입력하세요 (현재: " << row[11] << "): ";
            string assignmentScore;
            cin >> assignmentScore;

            cout << "출결 점수를 입력하세요 (현재: " << row[12] << "): ";
            string attendanceScore;
            cin >> attendanceScore;

            // 점수 수정 (입력 받은 값을 int로 변환하여 다시 string으로 저장)
            row[9] = to_string(safe_stoi(midtermScore));  // 중간 점수 수정
            row[10] = to_string(safe_stoi(finalScore));   // 기말 점수 수정
            row[11] = to_string(safe_stoi(assignmentScore));  // 과제 점수 수정
            row[12] = to_string(safe_stoi(attendanceScore));  // 출결 점수 수정

            cout << "성적이 수정되었습니다." << endl;
            cout << "총점: " << row[13] << endl;
            cout << "성적: " << row[14] << endl;
            break;
        }
    }

    // 수정된 데이터를 파일에 다시 저장
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "파일을 열 수 없습니다: " << filename << endl;
        return;
    }

    // 컬럼명 출력
    for (const auto& col : columnNames) {
        outFile << col << ",";
    }
    outFile << endl;

    // 수정된 데이터 저장
    for (const auto& values : data) {
        for (const auto& val : values) {
            outFile << val << ",";
        }
        outFile << endl;
    }
    outFile.close();

    // 수정된 내용을 확인
    cout << "수정된 학생 정보:" << endl;
    for (const auto& row : data) {
        if (safe_stoi(row[5]) == studentId && safe_stoi(row[4]) == professorId) {  // 학생 ID와 교수 ID가 일치하는 경우
            cout << "과목: " << row[1] << ", 학생명: " << row[3] << ", 학기: " << row[6] << ", 중간: " << row[9] << ", 기말: " << row[10] << ", 과제: " << row[11] << ", 출결: " << row[12] << endl;
        }
    }
    // 프로그램 종료 여부 묻기
    char choice;
    cout << "프로그램을 종료하시겠습니까? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        exit(0);  // 프로그램 종료
    }

    if (!found) {
        cout << "선택한 과목에 대한 학생이 없습니다." << endl;
        return;
    }
}


// 관리자 기능 - 수정
// 관리자 클래스
class Administrator {
private:
    class SubjectForAd {
    private:
        string subjectName, gradingSystem;   // 과목명, 성적 평가 제도
        int credit, capacity, enrollment, seme;   // 학점, 정원, 수강인원, 학기
        string professor;                   // 교수 이름 추가

    public:
        SubjectForAd(const string& subjectName, int credit, int capacity, int enrollment, const string& gradingSystem, const string& professor, int semester)
            : subjectName(subjectName), credit(credit), capacity(capacity), enrollment(enrollment), gradingSystem(gradingSystem), professor(professor), seme(semester) {}

        // Getter 메서드
        string getSubjectName() const { return subjectName; }
        int getCredit() const { return credit; }
        int getCapacity() const { return capacity; }
        int getEnrollment() const { return enrollment; }
        string getGradingSystem() const { return gradingSystem; }
        int getSemester() const { return seme; }
        string getProfessor() const { return professor; }

        // Setter 메서드
        void setEnrollment(int enrollment) { this->enrollment = enrollment; }
        void setGradingSystem(const string& gradingSystem) { this->gradingSystem = gradingSystem; }
    };

    vector<SubjectForAd> courses;
    vector<SubjectForAd> a_courses;

public:
    void loadCoursesFromCSV(const string& filePath) {
        courses.clear();
        a_courses.clear();
        ifstream file(filePath.c_str());

        if (!file.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePath << endl;
            return;
        }

        string line;
        getline(file, line); // 첫 줄 (헤더) 건너뛰기

        map<string, int> enrollmentCounts;
        map<string, int> credits;
        map<string, string> gradingSystems;
        map<string, string> professors;

        while (getline(file, line)) {
            stringstream ss(line);
            string subjectName, gradingSystem, semester, creditStr, professor;

            ss.ignore(100, ',');
            getline(ss, subjectName, ',');
            getline(ss, professor, ',');
            ss.ignore(100, ',');
            ss.ignore(100, ',');
            ss.ignore(100, ',');
            getline(ss, semester, ',');
            getline(ss, gradingSystem, ',');
            getline(ss, creditStr, ',');

            semester.erase(remove(semester.begin(), semester.end(), ' '), semester.end());

            int credit = 0;
            try {
                credit = stoi(creditStr);
            }
            catch (...) {
                cerr << "학점을 숫자로 변환할 수 없습니다: " << creditStr << endl;
                credit = 0;
            }

            int seme = stoi(semester);

            enrollmentCounts[subjectName]++;
            credits[subjectName] = credit;
            gradingSystems[subjectName] = gradingSystem;
            professors[subjectName] = professor;

            // a_courses에 강의 추가 (학기와 교수 기준으로 중복 확인)
            bool existsInACourses = false;
            for (const auto& course : a_courses) {
                if (course.getSubjectName() == subjectName && course.getProfessor() == professor && course.getSemester() == seme) {
                    existsInACourses = true;
                    break;
                }
            }

            if (!existsInACourses) {
                a_courses.emplace_back(subjectName, credit, 40, enrollmentCounts[subjectName], gradingSystem, professor, seme);
            }

            // 20242학기 강의만 courses에 추가
            if (seme == 20242) {
                bool existsInCourses = false;
                for (const auto& course : courses) {
                    if (course.getSubjectName() == subjectName && course.getProfessor() == professor && course.getSemester() == seme) {
                        existsInCourses = true;
                        break;
                    }
                }

                if (!existsInCourses) {
                    courses.emplace_back(subjectName, credit, 40, enrollmentCounts[subjectName], gradingSystem, professor, seme);
                }
            }
        }

        file.close();
    }


    void displayCourses() {
        if (courses.empty()) {
            cout << "금학기 개설된 과목 데이터가 없습니다." << endl;
            return;
        }

        cout << "금학기 개설된 과목은 다음과 같습니다:" << endl;
        cout << string(90, '-') << endl;
        cout << left << setw(30) << "과목명"
            << setw(8) << "학점"
            << setw(20) << "수강 가능 인원"
            << setw(12) << "수강 인원"
            << setw(15) << "평가 제도" << endl;
        for (const auto& course : courses) {
            cout << left << setw(30) << course.getSubjectName()
                << setw(8) << course.getCredit()
                << setw(20) << course.getCapacity()
                << setw(12) << course.getEnrollment()
                << setw(15) << course.getGradingSystem() << endl;
        }
        cout << string(90, '-') << endl;
    }

    void displayAllCourses() {
        cout << "모든 학기의 개설된 과목은 다음과 같습니다:" << endl;
        cout << string(100, '-') << endl;
        cout << left << setw(30) << "과목명"
            << setw(8) << "교수"
            << setw(8) << "학기"
            << setw(8) << "학점"
            << setw(20) << "수강 가능 인원"
            << setw(12) << "수강 인원"
            << setw(15) << "평가 제도" << endl;
        for (const auto& a_course : a_courses) {
            cout << left << setw(30) << a_course.getSubjectName()
                << setw(8) << a_course.getProfessor()
                << setw(8) << a_course.getSemester()
                << setw(8) << a_course.getCredit()
                << setw(20) << a_course.getCapacity()
                << setw(12) << a_course.getEnrollment()
                << setw(15) << a_course.getGradingSystem() << endl;
        }
        cout << string(100, '-') << endl;
    }

    void modifyAndDisplayCourses() {
        cout << "수정할 과목을 선택하세요:" << endl;
        for (size_t i = 0; i < courses.size(); ++i) {
            cout << i + 1 << ". " << courses[i].getSubjectName() << "-" << courses[i].getProfessor() << endl;
        }

        int choice;
        cout << "과목 번호를 입력하세요: ";
        cin >> choice;

        if (choice < 1 || choice > static_cast<int>(courses.size())) {
            cerr << "잘못된 선택입니다." << endl;
            return;
        }

        SubjectForAd& selectedCourse = courses[choice - 1];
        int newEnrollment;
        cout << "\"" << selectedCourse.getSubjectName() << "\"의 새로운 수강 인원을 입력하세요: ";
        cin >> newEnrollment;

        selectedCourse.setEnrollment(newEnrollment);
        string newGradingSystem;
        if (newEnrollment >= 15) newGradingSystem = "Normal";
        else if (newEnrollment >= 10) newGradingSystem = "English A";
        else newGradingSystem = "P/F";

        selectedCourse.setGradingSystem(newGradingSystem);
        displayCourses();
    }
};





int main() {
    //프로그램 시작
    cout << R"(









*************************************************************************
*************************************************************************

                                                            
                            @@      ,,     .,     @*                  
 ~@@@@@, @$  @@@@@@@@@  @@@@@@@@@@  @@     $@  @@@@@@@ @   @@@@@@ @@  
     .@  @$         @@    =$$$$$;   @@     $@          @       @@ @@  
   @ #@  @@@        @@   @@@@@@@@@  @@@@@@@@@   @@@@@  @@@     @@ @@  
   @ @@  @$     @@  @@   @@@@@@@@@  @@     $@   @   @@ @       @@ @@- 
 @@@@@@@ @$     @@  @@      @@      @@@@@@@@@   @@@@@  @    @; @@ @@@@
 ======= @$     @@  @@  @@@@@@@@@@@ @@@@@@@@@               @; @! @@  
  @@@@@@@@      @@                      --      @@@@@@@@    @; @  @@  
  @@....:@@     @@       @@@@@@@@@      @@             @    @;    @@  
  @$     @@  @@@@@@@@@@  @      @@ ,@@@@@@@@@@         @  $@@@@@@ @@  
  @@@@@@@@   @@@@@@@@@@  @@@@@@@@@ ,@@@@@@@@@@         @  $@@@@@@ @@  
                                                                      
                                                                      
                                                                      
    @@   @  @@@@@  @  @@@@@ @   @@@@@ @@   @@   @!    @@     @@@@ @ @ 
    @    @   $@    @     @@ @      @@ @@   @    @!    @@     @@   @ @ 
   @@@ @@@   @@@ @@@   - @@ @@@    @@ @@   @    @!   @@@@    @@@ @@ @ 
   @ @@  @   @ @.  @   @ @@ @      @@ @@  @@    @!  .@ #@    @@   @ @ 
  @@  @  @  @@ @@  @ @@@@@@ @   *@@@@ @@  @@@   @!  @@  @@   @@@@ @ @ 
                     @@@@@@ @   @@    @@  @,@   @! @@    @@           
   @@@@@@@  @@@@@@@@  @     @   @     @@  @ @@  @!           @@@@@@@@ 
   @     @@        @  @         @     @@ @@  @  @!           @$     @ 
   @@@@@@@         @  @@@@@@@@  @@@@@ @@ @@  @@ @! @@@@@@@@@ @@@@@@@@ 
                                                                      
                                                                      
*************************************************************************
*************************************************************************
                                                            
    )" << endl;

    //로그인
    // 
    string userInfoFile = "User_Info.csv"; //사용자 로그인 정보 파일
    vector<UserInfo> users = loadUserInfo(userInfoFile);

    if (users.empty()) {
        cerr << "사용자 정보를 로드하지 못했습니다." << endl;
        return 1;
    }

    string subjectFile = "Subject_Info.csv"; // 과목 정보 파일
    vector<vector<string>> data = loadData(subjectFile);

    if (data.empty()) {
        cerr << "데이터를 로드할 수 없습니다." << endl;
        return 1;
    }

    // 사용자 입력
    int userType;
    cout << "신분을 입력하세요 (1: 학생, 2: 교수, 3: 관리자): ";
    cin >> userType;

    if (userType < 1 || userType > 3) {
        cout << "잘못된 신분입니다." << endl;
        return 1;
    }

    string password;
    int id;
    cout << "ID: ";
    cin >> id;
    cout << "비밀번호: ";
    cin >> password;

    // 로그인 확인
    while (!login(users, userType, id, password)) {
        cout << "로그인 실패! 정보가 일치하지 않습니다." << endl;

        char choice;
        cout << "다시 시도하시겠습니까? (Y/N): ";
        cin >> choice;

        if (choice == 'N' || choice == 'n') {
            cout << "프로그램을 종료합니다." << endl;
            return 1;
        }

        cout << "ID: ";
        cin >> id;
        cout << "비밀번호: ";
        cin >> password;
    }

    cout << "로그인 성공!" << endl;

    // 역할별 기능
    string userId = to_string(id); // ID를 문자열로 변환

    if (userType == 1) {  // 학생
        cout << "학생 모드에 진입합니다." << endl;
        cout << "목록에서 기능을 선택하세요" << endl;
        while (true) {
            cout << "1. 성적 조회" << endl;
            cout << "2. 종료" << endl;
            int choice;
            cin >> choice;
            switch (choice) {
            case 1:
                cout << "학생 ID " << id << "의 성적 정보를 조회합니다." << endl;
                cout << "   년도/학기   |      과목명      |   학점   |   성적   " << endl;
                displayStudentSubjects(data, id); // 교수의 ID와 비교
                break;
            case 2:
                cout << "프로그램을 종료합니다." << endl;
                return 0;  // 프로그램 종료
            default:
                cout << "잘못된 선택입니다. 다시 시도해주세요." << endl;
            }
        }
        cout << "학생 ID " << id << "의 성적을 조회합니다." << endl;
        displayStudentSubjects(data, id); // 학생의 ID와 비교
    }
    else if (userType == 2) {  // 교수
        cout << "교수 모드에 진입합니다." << endl;
        cout << "목록에서 기능을 선택하세요" << endl;
        while (true) {
            cout << "1. 과목 조회" << endl;
            cout << "2. 성적 수정" << endl;
            cout << "3. 종료" << endl;

            int choice;
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "교수 ID " << id << "의 강의 정보를 조회합니다." << endl;
                cout << "   년도/학기   |      과목명      |   학생   |   성적   " << endl;
                displayProfessorSubjects(data, id); // 교수의 ID와 비교
                break;
            case 2:
                cout << "성적 정보를 수정합니다." << endl;
                // 교수 성적 수정 기능 호출
                reviseProfessorSubjects(subjectFile, id, data, data[0]); // data[0]은 컬럼명
                break;
            case 3:
                cout << "프로그램을 종료합니다." << endl;
                return 0;  // 프로그램 종료
            default:
                cout << "잘못된 선택입니다. 다시 시도해주세요." << endl;
            }
        }
    }
    else if (userType == 3) {  // 관리자
        Administrator admin;
        admin.loadCoursesFromCSV(subjectFile);
        cout << "관리자 모드에 진입합니다." << endl;
        cout << "목록에서 기능을 선택하세요" << endl;
        while (true) {
            cout << "1. 과목 조회" << endl;
            cout << "2. 과목 수정" << endl;
            cout << "3. 종료" << endl;
            int choice;
            cin >> choice;
            switch (choice) {
            case 1:// 년도/학기, 과목명, 교수, 학생, 학점, 성적
                cout << "모든 강의 정보를 조회합니다." << endl;
                admin.displayAllCourses();
                break;
            case 2:
                cout << "과목 정보를 수정합니다." << endl;
                admin.modifyAndDisplayCourses();
                break;
            case 3:
                cout << "프로그램을 종료합니다." << endl;
                return 0;  // 프로그램 종료
            default:
                cout << "잘못된 선택입니다. 다시 시도해주세요." << endl;
            }
        }
        displayAllSubjects(data); // 모든 정보를 출력
    }
    return 0;
}