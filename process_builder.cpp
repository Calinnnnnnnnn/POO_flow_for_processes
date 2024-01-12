#include <iostream>
#include <cstring>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <map>
using namespace std;

//clasa principala din care sunt mostenite metodele pentru steps
class ProcessSteps{
    public:
        virtual void execute() = 0; //executa pasi la parcurgere
        virtual void execute_newFlow() = 0; //executa pasi la creare
        virtual void description_step() = 0; //descrierea pasului
        virtual string get_step_type() const = 0; //ofera tipul pasului
        virtual string getFileName() const = 0; //numele fisierului furnizat daca e cazul
        virtual string get_data() const = 0; //preia datele la parcurgere
        virtual string get_data_newFlow() const = 0; //preia datele la creare
        virtual string get_content() const = 0; //preia continutul unui pas
        virtual ~ProcessSteps() {} //destructor
};

vector<ProcessSteps*> steps; //vectorul unde sunt retinuti pasii
vector<string> operations_clc = {"Addition", "Substraction", "Multiplication", "Division", "Minimum", "Maximum"}; //vector cu tipurile de operatii pentru Calculus
int txt_present = 0, csv_present = 0, started_flow = 0; //contori pentru indicarea prezentei unui fisier txt/csv 
                                                                //si indicarea inceperii unei parcurgei a unui flow
int loop = 0; //variabila pentru a intoarce utilizatorul la Main Menu

int completed_flow = 1, error_completedFlow = 0;
//variabila de verificare daca un flow este parcurs in totalitate +
//numar de erori pentru flow parcurs in totalitate

string text_input; //pentru a retine ce introduce utilizatorul in cazul TIS, NIS, CS
float number_input;
int step_calculus;

//functie pentru actualizarea fisierului ce contine media de erori
//pentru un flow parcurs in totalitate
void errors_completed_flows(const string name_of_flow)
{
    std::ifstream file("errors_completed_flows.csv"); //fisierul cu flow-urile si numarul de erori pentru flow complet
    if(file.is_open())
    {
        std::string line;
        std::vector<std::string> lines;

        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string flowName, errorValue;

            if(std::getline(iss, flowName, ',') && std::getline(iss, errorValue, ','))
            {
                int errorInt = std::stoi(errorValue);
                if(flowName == name_of_flow) //cauta flow-ul dat
                   errorInt = (errorInt + error_completedFlow) / 2; //modifica numarul de erori, medie de erori per flow complet
                std::ostringstream oss;
                oss << flowName << ',' << errorInt << "\n";
                lines.push_back(oss.str()); //retine fiecare linie din csv cu sau fara modificari
            }
            
        }

        file.close();
        std::ofstream outputFile("errors_completed_flows.csv");
        if(outputFile.is_open())
        {
            for(const auto& updatedLine: lines)
                outputFile << updatedLine; //scrie fisierul actualizat
            outputFile.close();
        }
        else std::cerr << "Error opening output file 'errors_completed_flows.csv'\n";
    }else std::cerr << "Error opening file 'errors_completed_flows.csv'\n";
}

//functia ce actualizeaza fisierul ce contine numarul
//de pasi carora li se da skip
void errors_steps(const string name_of_step)
{
    std::ifstream file("errors_steps.csv");
    if(file.is_open())
    {
        std:;string line;
        std::vector<std::string> lines;

        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string stepName, skippedValue;

            if(std::getline(iss, stepName, ',') && std::getline(iss, skippedValue, ','))
            {
                int skippedInt = std::stoi(skippedValue); //se retine valoarea din fisier si se converteste in int
                if(stepName == name_of_step)
                    skippedInt++; //se incrementeaza in cazul in care pasului i se da skip
                std::ostringstream oss;
                oss << stepName << ',' << skippedInt << "\n";
                lines.push_back(oss.str());
            }
            
        }

        file.close();
        std::ofstream outputFile("errors_steps.csv");
        if(outputFile.is_open())
        {
            for(const auto& updatedLine: lines)
                outputFile << updatedLine; //scrie fisierul actualizat
            outputFile.close();
        }
        else std::cerr << "Error opening output file 'errors_steps.csv'\n";
    }else std::cerr << "Error opening file 'errors_steps.csv'\n";
}


//explicatiile pentru metode apar aici in TitleStep
class TitleStep: public ProcessSteps{
    private:
        string title, subtitle;
    public:
        TitleStep(const string titlu, const string subtitlu)
        {
            this->title = titlu;
            this->subtitle = subtitlu;
        }

        //executie la parcurgerea unui flow
        void execute() override{
            std::cout << "\nTitle Step\n";
            std::cout << "Title: " << title << '\n';
            std::cout << "Subtitle: " << subtitle << '\n';
        }

        //executie la crearea unui nou flow
        void execute_newFlow() override{
            std::cout << "\nTitle Step\n";
            std::cout << "Title: " << title << '\n';
            std::cout << "Subtitle: " << subtitle << '\n';
        }

        //metoda pentru descrierea pasului la cererea utilizatorului
        void description_step() override{
            std::cout << "\t-->This step if for adding a title and a subtitle to the flow\n";
            std::cout << "\t-->You must add strings for Title and Subtitle\n";
        }

        string get_step_type() const override{
            return "TitleStep"; //tipul pasului
        }
        string getFileName() const override{
            return "dummy"; //metoda doar trebuie mostenita
        }       
        string get_data() const override{
            return "Title Step\nTitle: " + title + "\nSubtitle: " + subtitle + "\n"; //datele pe care le contine clas
        }
        string get_data_newFlow() const override{
            return "Title Step\nTitle: " + title + "\nSubtitle: " + subtitle + "\n";; //datele folosite la afisarea tuturor pasilor
        }
        string get_content() const override{
            return "Title Step,Title: " + title + ",Subtitle: " + subtitle; //datele pentru scrierea in fisierul flows_expanded.csv
        }
        ~TitleStep() {}
};

class TextStep: public ProcessSteps{
    private:
        string title, copy;
    public:
        TextStep(const string titlu, const string copie)
        {
            this->title = titlu;
            this->copy = copie;
        }

        void execute() override{
            std::cout << "\nText Step\n";
            std::cout << "Title: " << title << '\n';
            std::cout << "Copy: " << copy << '\n'; 
        }

        void execute_newFlow() override{
            std::cout << "\nText Step\n";
            std::cout << "Title: " << title << '\n';
            std::cout << "Copy: " << copy << '\n'; 
        }

        void description_step() override{
            std::cout << "\t-->This step is for more informations about flow you create\n";
            std::cout << "\t-->You must add strings for Title and Copy\n";
        }

        string get_step_type() const override{
            return "TextStep";
        }

        string getFileName() const override{
            return "dummy";
        }

        string get_data() const override{
            return "Text Step\nTitle: " + title + "\nCopy: " + copy + "\n";
        }
        string get_data_newFlow() const override{
            return "Text Step\nTitle: " + title + "\nCopy: " + copy + "\n";;
        }
        string get_content() const override{
            return "Text Step,Title: " + title +",Copy: " + copy;
        }
        ~TextStep() {}
};

class TextInputStep: public ProcessSteps{
    private:
        string description, textInput;
    public:
        TextInputStep(const string descriere, const string text_input)
        {
            this->description = descriere;
            this->textInput = text_input;
        }

        TextInputStep(const string descriere)
        {
            this->description = descriere;
        }

        void execute() override{
            std::cout << "\nText Input Step\n";
            std::cout << "Description: " << description << '\n';
            std::cout << "Text Input: " << textInput << '\n';
        }
        void execute_newFlow() override{
            std::cout << "\nText Input Step\n";
            std::cout << "Description: " << description << '\n';
        }

        void description_step() override{
            std::cout << "\t-->This step is for datas that you want to be in the flow\n";
            std::cout << "\t!!!If you don't introduce datas at this step the process will stop!!!\n";
            std::cout << "\t-->You must add strings for Description and Text Input\n";
        }
        string get_step_type() const override{
            return "TextInputStep";
        }

        string getFileName() const override{
            return "dummy";
        }
        string get_data() const override{
            return "Text Input Step\nDescription: " + description + "\nText Input: " + textInput + "\n"; 
        }

        string get_data_newFlow() const override{
            return "Text Input Step\nDescription: " + description + "\n"; 
        }
        string get_content() const override{
            return "Text Input Step,Description: " + description;
        }
        ~TextInputStep() {}
};

class NumberInputStep: public ProcessSteps{
    private:
        string description;
        float numberInput;
    public:
        NumberInputStep(const string descriere, float numar)
        {
            this->description = descriere;
            this->numberInput = numar;
        }
        NumberInputStep(const string descriere)
        {
            this->description = descriere;
        }

        void execute() override{
            std::cout << "\nNumber Input Step\n";
            std::cout << "Description: " << description << '\n';
            std::cout << "Number Input: " << numberInput << '\n';
        }

        void execute_newFlow() override{
            std::cout << "\nNumber Input Step\n";
            std::cout << "Description: " << description <<'\n';
        }

        void description_step() override{
            std::cout << "\t-->This step is for adding number datas that you want to be in your flow\n";
            std::cout << "\t-->You must add a float number for Number Input and a string for Description\n";
        }

        string get_step_type() const override{
            return "NumberInputStep";
        }

        string getFileName() const override{
            return "dummy";
        }
        string get_data() const override{
            return "Number Input Step\nDescription: " + description + "\nNumber Input: " + std::to_string(numberInput) + "\n";
        }

        string get_data_newFlow() const override{
            return "Number Input Step\nDescription: " + description + "\n";
        }
        string get_content() const override{
            return "Number Input Step,Description: " + description;
        }
        ~NumberInputStep(){}
};

class CalculusStep: public ProcessSteps{
    private:
        int step;
        string operation;
        float result;
    public:
        CalculusStep(int pasi, const string operatie, float result)
        {
            this->step = pasi;
            this->operation = operatie;
            this->result = result;
        }
        CalculusStep(const string operatie)
        {
            this->operation = operatie;
        }

        void execute() override{
            std::cout << "\nCalculus Step\n";
            std::cout << "Step: " << step << '\n';
            std::cout << "Operation: " << operation << '\n';
            std::cout << "Result: " << result << '\n';
        }

        void execute_newFlow() override{
            std::cout << "\nCalculus Step\n";
            std::cout << "Operation: " << operation << '\n';
        }
        void description_step() override{
            std::cout << "\t-->This step is for operations that you want to do with the numbers from Number Input Step\n";
            std::cout << "\tOperations:\n";
            std::cout << "\t\t-->Addition(+)\n\t\t-->Substraction(-)\n\t\t-->Multiplication(*)\n\t\t-->Division(/)\n\t\t-->Minimum(min)\n\t\t-->Maximum(max)\n";
            std::cout << "\t-->You must add an integer number for Steps and a string for Operation\n";
        }

        string get_step_type() const override{
            return "CalculusStep";
        }

        string getFileName() const override{
            return "dummy";
        }
        string get_data() const override{
            return "Calculus Step\nSteps: " + std::to_string(step) + "\nOperation: " + operation + "\nResult: " + std::to_string(result) + "\n";
        }
        string get_data_newFlow() const override{
            return "Calculus Step\nOperation: " + operation + "\n";
        }
        string get_content() const override{
            return "Calculus Step,Operation: " + operation;
        }
        ~CalculusStep(){}
};

class DisplayStep: public ProcessSteps{
    private:
        int step;
        string filename;
    public:
        DisplayStep(int pas)
        {
            this->step = pas;
        }

        void execute() override{
            int index = 0;
            std::cout << "\nDisplay Step\n";
            std::cout << "Information from step: " << step << "(1 for txt, 2 for csv)\n";
            if(step == 1)
            {
                std::cout << "Information for txt file:\n";
                for(const auto& step: steps)
                    if(step->get_step_type() == "TextFileStep") //cauta pasul TextFileStep pentru a lua datele din el si 
                    {                                           //a afisa continutul fisierului furnziat
                        filename = step->getFileName();
                        ifstream file(filename);
                        std::cout << "File Name: " << filename << '\n';
                        if(file.is_open())
                        {
                            string line;
                            std::cout << "File content:\n";

                            while(std::getline(file, line))
                                std::cout << line << '\n';
                            
                            file.close();
                        }
                    }
            }
            else
            {
                std::cout << "Information for csv file:\n";
                for(const auto& step: steps)
                    if(step->get_step_type() == "CSVFileStep") //cauta pasul CSVFileStep pentru a lua datele din el si 
                    {                                          //a afisa continutul fisierului furnziat
                        filename = step->getFileName();
                        ifstream file(filename);
                        std::cout << "File Name: " << filename << '\n';
                        if(file.is_open())
                        {
                            string line;
                            std::cout << "File content:\n";

                            while(std::getline(file, line))
                                std::cout << line << '\n';
                            
                            file.close();
                        }
                    }
            }
            
        }

        void execute_newFlow() override{
            std::cout << "\nDisplay Step\nStep added to flow!\n";
        }

        void description_step() override{
            std::cout << "\t-->This step gives you information about any steps that you used before\n";
            std::cout << "\t-->You must add an integer number for Step\n";
        }

        string get_step_type() const override{
            return "DisplayStep";
        }

        string getFileName() const override{
            return "dummy";
        }
        string get_data() const override 
        {
            std::string data = "Display Step\nInformation from step (1 for txt, 2 for csv): " + std::to_string(step) + "\nFile Name: " + filename + "\n";

            ifstream file(filename);
            if (file.is_open()) 
            {
                std::string line;
                data += "File content:\n";
                
                while (std::getline(file, line)) 
                    data += line + '\n';

                file.close();
            } 
            else 
            {
                data += "Unable to open file: " + filename + "\n";
            }
            return data;
        }
        string get_data_newFlow() const override{
            return "Display Step\nReady to read files\n";
        }
        string get_content() const override{
            return "Display Step,Ready to read files";
        }
        ~DisplayStep() {}
};

//cuprinde explicatiile pentru metodele de la TextFileStep + CSVFileStep
class TextFileStep: public ProcessSteps{
    private:
        string description, fileName, answer;
        int index = 0;
    public:
        TextFileStep(const string descriere, const string file_nume)
        {
            this->description = descriere;
            this->fileName = file_nume;
        }

        TextFileStep(const string descriere)
        {
            this->description = descriere;
        }

        void execute() override{
            int ok = 0;
            std::cout << "\nText File Step\n";
            std::cout << "Description: " << description << '\n';
            
            while(ok == 0)
            {     
                if(index == 1) //pentru cazul in care nu exista fisierul furnizat
                {
                    std::cerr << "File Name: ";
                    getline(cin, fileName);
                }
                try
                {
                    ifstream file(fileName);
                    std::cout << "File Name: " << fileName << '\n';
                    if(file.is_open())
                    {
                        std::cout << "->Status: OK!\n";
                        file.close();
                        txt_present = 1;
                        ok = 1;
                    }
                    else throw std::runtime_error("Unable to open file: " + fileName);
                }
                catch(const std::exception& e)
                {
                    error_completedFlow++; //in caz de eroare se incrementeaza aceasta variabila pentru cazul in care
                                           //flow-ul ce contine pasul este parcurs complet
                    errors_steps("TextFileStep"); //pentru a modifica numarul de erori pentru acest pas in fsier
                    std::cerr << "\nError! " <<  e.what() << '\n';
                    std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                    std::cerr << "Answer: ";
                    std::getline(cin, answer); //posibilitatea de a incerca iar introducerea unui fiiser valid
                    if(answer == "yes")
                    {
                        ok = 0;
                        index = 1;
                    }
                    else
                    {
                        ok = 1;
                        completed_flow = 0;
                    }
                }
            }

        }

        void execute_newFlow() override{
            std::cout << "\nText File Step\n";
            std::cout << "Description: " << description << '\n';
            txt_present = 1;
        }

        void description_step() override{
            std::cout << "\t-->This step adds a TXT file to the flow\n";
            std::cout << "\t-->You must add strings for the Description and File Name\n";
        }

        string get_step_type() const override{
            return "TextFileStep";
        }

        string getFileName() const override{
            return fileName;
        }

        string get_data() const override{
            return "Text File Step\nDescription: " + description + "\nFile Name: " + fileName + "\n";
        }
        string get_data_newFlow() const override{
            return "Text File Step\nDescription: " + description + "\n";
        }
        string get_content() const override{
            return "Text File Step,Description: " + description;
        }
        ~TextFileStep() {}
};

class CSVFileStep: public ProcessSteps{
    private:
        string description, fileName, answer;
        int index = 0;
    public:
        CSVFileStep(const string descriere, const string file_nume)
        {
            this->description = descriere;
            this->fileName = file_nume;
        }

        CSVFileStep(const string descriere)
        {
            this->description = descriere;
        }

        void execute() override{
            int ok = 0;
            std::cout << "\nCSV File Input Step\n";
            std::cout << "Description: " << description << '\n';

            while(ok == 0)
            {     
                if(index == 1)
                {
                    std::cerr << "File Name: ";
                    getline(cin, fileName);
                }
                try
                {
                    ifstream file(fileName);
                    std::cout << "File Name: " << fileName << '\n';
                    if(file.is_open())
                    {
                        std::cout << "->Status: OK!\n";
                        file.close();
                        csv_present = 1;
                        ok = 1;
                    }
                    else throw std::runtime_error("Unable to open file: " + fileName);
                }
                catch(const std::exception& e)
                {
                    error_completedFlow++;
                    errors_steps("CSVFileStep");
                    std::cerr << "Error! " <<  e.what() << '\n';
                    std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                    std::cerr << "Answer: ";
                    std::getline(cin, answer);
                    if(answer == "yes")
                    {
                        ok = 0;
                        index = 1;
                    }
                    else
                    {
                        ok = 1;
                        completed_flow = 0;
                    }
                }
            }
        }

        void execute_newFlow() override{
            std::cout << "\nCSV File Input Step\n";
            std::cout << "Description: " << description << '\n';
            csv_present = 1;
        }

        void description_step() override{
            std::cout << "\t-->This step adds a CSV file to the flow\n";
            std::cout << "\t-->You must add strings for the Description and File Name\n";
        }

        string get_step_type() const override{
            return "CSVFileStep";
        }

        string getFileName() const override{
            return fileName;
        }

        string get_data() const override{
            return "CSV File Step\nDescription: " + description + "\nFile Name: " + fileName + "\n";
        }

        string get_data_newFlow() const override{
            return "CSV File step\nDescription: " + description + "\n";
        }
        string get_content() const override{
            return "CSV File Step,Description: " + description;
        }
        ~CSVFileStep() {}
};

class OutputStep: public ProcessSteps{
    private:
        int step;
        string fileName, title, description;
    public:
        OutputStep(int pas, const string file_nume, const string titlu, const string descriere)
        {
            this->step = pas;
            this->fileName = file_nume;
            this->title = titlu;
            this->description = descriere;
        }

        void execute() override{
            std::cout << "\nOutput Step\n";
            std::cout << "Step: " << step << '\n';
            std::cout << "File name: " << fileName << '\n';
            std::cout << "Title: " << title << '\n';
            std::cout << "Description: " << description << '\n';

            std::ofstream outputFile(fileName); //scrierea in fisierul dat la crearea flow-ului

            if(outputFile.is_open())
            {
                outputFile << "Title: " << title << '\n';
                outputFile << "Description: " << description << "\n\n";
                int index = 0;
                for(const auto& pas: steps)
                {
                    if(index == step - 1)
                    {
                        outputFile << pas->get_data(); //afiseaza datele pe care le contine un pas anterior ce a fost selectat
                        break;
                    }
                    else index++;
                }

                outputFile.close();
                std::cout << "Output file created successfully!\n";
            }
            else  std::cerr << "Error opening output file: " << fileName << '\n';
        }

        void execute_newFlow() override{
           std::cout << "\nOutput Step\n";
            std::cout << "Step: " << step << '\n';
            std::cout << "File name: " << fileName << '\n';
            std::cout << "Title: " << title << '\n';
            std::cout << "Description: " << description << '\n';

            std::ofstream outputFile(fileName);

            if(outputFile.is_open())
            {
                outputFile << "Title: " << title << '\n';
                outputFile << "Description: " << description << "\n\n";
                int index = 0;
                for(const auto& pas: steps)
                {
                    if(index == step - 1)
                    {
                        outputFile << pas->get_data();
                        break;
                    }
                    else index++;
                }

                outputFile.close();
                std::cout << "Output file created successfully!\n";
            }
            else  std::cerr << "Error opening output file: " << fileName << '\n';
        }

        void description_step() override{
            std::cout << "\t-->This step creates an output file that contains datas from a specified step\n";
            std::cout << "\t-->You must add an integer for Step and strings for File Name, Title and Subtitle\n";
        }

        string get_step_type() const override{
            return "OutputStep";
        }
        
        string getFileName() const override{
            return "dummy";
        }
        string get_data() const override{
            return "Output Step\nStep: " + std::to_string(step) + "\nFile name: " + fileName + "\nTitle: " + title + "\nDescription: " + description + "\n";
        }
        string get_data_newFlow() const override{
            return "Output Step\nStep: " + std::to_string(step) + "\nFile name: " + fileName + "\nTitle: " + title + "\nDescription: " + description + "\n";;
        }
        string get_content() const override{
            return "Output Step,Step: " + std::to_string(step) + ",File name: " + fileName + ",Title: " + title + ",Description: " + description;
        }
        ~OutputStep() {}
};

class EndStep: public ProcessSteps{
    public:
        void execute() override{
            std::cout << "\nEnd of the process!\n";
        }

        void execute_newFlow() override{
            std::cout << "\nEnd of the process!\n";
        }

        void description_step() override{
            std::cout << "\t-->This step ends your flow creation\n";
            std::cout << "\t-->This is the end of the Flow\n";
        }

        string get_step_type() const override{
            return "EndStep";
        }

        string getFileName() const override{
            return "dummy";
        }
        string get_data() const override{
            return "End Step\nEnd of the process!\n";
        }
        string get_data_newFlow() const override{
            return "End Step\nEnd of the process!\n";
        }
        string get_content() const override{
            return "End Step,End of the processes!";
        }
        ~EndStep() {}
};

//functie pentru descrierea functionalitatii pasilor
void descriptionOfSteps(const string answer) 
{
    if (answer == "Title Step details") {
        TitleStep title_step("titlu", "subtitlu");
        std::cout << "Details for Title Step:\n";
        title_step.description_step();
    }
    else if (answer == "Text Step details") {
        TextStep text_step("titlu", "copie");
        std::cout << "Details for Text Step:\n";
        text_step.description_step();
    }
    else if (answer == "Text Input Step details") {
        TextInputStep text_input_step("descriere", "text_input");
        std::cout << "Details for Text Input Step:\n";
        text_input_step.description_step();
    }
    else if (answer == "Number Input Step details") {
        NumberInputStep number_input_step("descriere", 42.0); 
        std::cout << "Details for Number Input Step:\n";
        number_input_step.description_step();
    }
    else if (answer == "Calculus Step details") {
        CalculusStep calculus_step(5, "addition", 2); 
        std::cout << "Details for Calculus Step:\n";
        calculus_step.description_step();
    }
    else if (answer == "Display Step details") {
        DisplayStep display_step(2); 
        std::cout << "Details for Display Step:\n";
        display_step.description_step();
    }
    else if (answer == "Text File Step details") {
        TextFileStep text_file_step("descriere", "file_name.txt");
        std::cout << "Details for Text File Step:\n";
        text_file_step.description_step();
    }
    else if (answer == "CSV File Step details") {
        CSVFileStep csv_file_step("descriere", "data.csv");
        std::cout << "Details for CSV File Step:\n";
        csv_file_step.description_step();
    }
    else if (answer == "Output Step details") {
        OutputStep output_step(3, "output_file.txt", "output_title", "output_description");
        std::cout << "Details for Output Step:\n";
        output_step.description_step();
    }
    else if (answer == "End Step details") {
        EndStep end_step;
        std::cout << "Details for End Step:\n";
        end_step.description_step();
    }
    else {
        std::cout << "Unknown step type or details not specified for: " << answer << '\n';
    }
}

//functie pentru afisarea semnficiatiilor fisierelor csv ce contin analize
void descriptionOfFlowsFiles(const string name)
{
    if(name == "errors_steps details")
    {
        std::cout << "\nDetails for: errors_steps.csv\n";
        std::cout << "The structure of the file is: step_name | the number of errors for each step\n";
    }
    else if(name == "skipped_steps details")
    {
        std::cout << "\nDetails for: skipped_steps.csv\n";
        std::cout << "The strcuture of the file is: step_name | the number of times that flow has been skipped\n";
    }
    else if(name == "errors_completed_flows details")
    {
        std::cout << "\nDetails for: errors_completed_flows.csv\n";
        std::cout << "The strcuture of the file is: flow_name | the number of errors per flow completed\n";
    }
    else if(name == "Flows_categories details")
    {
        std::cout << "\nDetails for: Flows_categories.csv\n";
        std::cout << "The strcuture of the file is: flow_name | the number of times that flow has been started | the number of times that flow has been completed\n";
    }
    else if(name == "flows details")
    {
        std::cout << "\nDetails for: flows.csv\n";
        std::cout << "The structure of the file is: flow_name | steps | date&time of creation | the number of uses\n";
    }
    else std::cout << "Unknown file or details not specified for: " << name << '\n';
}

//functie ce contine operatiile pentru CalculusStep si implementarea lor
template<typename T, typename U>
T operations_for_calculus(T n1, U n2, const string operation)
{
    if(operation == "Addition")
        return (n1 + n2);
    if(operation == "Substraction")
        return (n1 - n2);
    if(operation == "Multiplication")
        return (n1 * n2);
    if(operation == "Division")
        return (n1 / n2);
    if(operation == "Minimum")
    {
        if(n1 < n2)
            return n1;
        else return n2;
    }
    if(operation == "Maximum")
    {
        if(n1 > n2)
            return n1;
        else return n2;
    }
    return INT_MIN;
}

//clasa utilizata pentru crearea flow-ului
class FlowCreation{
    private:
        string name_of_the_flow;
        
    public:
        FlowCreation(string name)
        {
            this->name_of_the_flow = name;
        }

        //adaugarea in vectorul de pasi
        void add_steps(ProcessSteps* step){
            steps.push_back(step);
        }

        //executarea pasilor
        void execute_step(ProcessSteps* step){
            step->execute();
        }

        //executarea pasilor pentru un flou nou
        void execute_step_newFlow(ProcessSteps* step){
            step->execute_newFlow();
        }

        //afisrea pasilor dupa parcurgerea unui flow
        void display_all_steps(){
            std::ofstream output("All_flows_created.txt", std::ios::app);

            for(const auto& step: steps)
                step->execute();

            if(output.is_open())
            {
                output << "\nName of the flow: " << name_of_the_flow << "\n";
                for(const auto& step: steps)
                {
                    output << step->get_data() << "\n";
                }
                output << "\n";
                output.close();
                std::cout << "-->Output file completed successfully!\n";
            }
            else std::cerr << "Error creating file!\n";
        }

        //afisrea pasilor dupa crearea unui flow
        void display_all_steps_newFlow(){
            std::ofstream output("All_flows_created.txt", std::ios::app);

            for(const auto& step: steps)
                step->execute_newFlow();

            if(output.is_open())
            {
                output << "\nName of the flow: " << name_of_the_flow << "\n";
                for(const auto& step: steps)
                {
                    output << step->get_data_newFlow() << "\n";
                }
                output << "\n";
                output.close();
                std::cout << "\n!Output file created successfully!\n";
            }
            else std::cerr << "Error creating file!\n";
        }

        //aduagarea flow-ului in fisierul ce contine datele esentiale despre flow-uri
        void add_to_csv()
        {
            ofstream csv_file("flows.csv", ios::app);
        
            if (csv_file.is_open()) 
            {
                // Scrie numele flow-ului
                csv_file << name_of_the_flow << ",";

                for (const auto& step : steps) {
                    // Scrie tipul pasului în fișierul CSV
                    csv_file << step->get_step_type() << ",";
                }
                // Obține data și ora curentă
                time_t now = time(0);
                tm* ltm = localtime(&now);

                // Scrie data și ora în fișierul CSV
                csv_file << ltm->tm_mday << "-" << (ltm->tm_mon + 1) << "-" << (ltm->tm_year + 1900) << " ";
                csv_file << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;

                csv_file << "," << 0;
                csv_file << "\n";
                csv_file.close();
            } 
            else std::cerr << "Unable to open file: flows.csv\n";

            ofstream csv_file2("Flows_categories.csv", ios::app); //adaugare flow in fisierul pentru flow inceput-complet

            if(csv_file2.is_open())
            {
                csv_file2 << name_of_the_flow << ',' << 0  << ',' << 0;
                csv_file2 << "\n";
                csv_file2.close();
            }
            else std::cerr << "Unable to open file: Flows_categories.csv\n";

            ofstream csv_file3("errors_completed_flows.csv", ios::app); //adaugare flow in fisierul pentru  medie pe flow complet

            if(csv_file3.is_open())
            {
                csv_file3 << name_of_the_flow << ',' << 0 << "\n";
                csv_file3.close();
            }
            else std::cerr << "Unable to open file: errors_completed_flows.csv\n";
        }

        //adaugarea unui flow in fiserul ce contine toate datele despre flow: nume, pasi, datele continute de pasi
        void add_to_csv_expanded()
        {
            std::ofstream csv_file("flows_expanded.csv", std::ios::app);

            if(csv_file.is_open())
            {
                csv_file << name_of_the_flow << ",";
                for (size_t i = 0; i < steps.size(); i++) 
                {
                    // Scrie tipul pasului și conținutul în fișierul CSV
                    csv_file << steps[i]->get_step_type() << "," << steps[i]->get_content();

                    // Adaugă virgulă doar dacă nu este ultimul element
                    if (i < steps.size() - 1)
                        csv_file << ",";
                }
                csv_file << '\n';
            }
            else std::cerr << "Unable to open file: flows_expanded.csv\n";
        }

};

//functie ce actualizeaza fisierul pentru pasii carora li se da skip
void skipped_steps(const string name_of_step)
{
    std::ifstream file("skipped_steps.csv");
    if(file.is_open())
    {
        std:;string line;
        std::vector<std::string> lines;

        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string stepName, skippedValue;

            if(std::getline(iss, stepName, ',') && std::getline(iss, skippedValue, ','))
            {
                int skippedInt = std::stoi(skippedValue);
                if(stepName == name_of_step)
                    skippedInt++;
                std::ostringstream oss;
                oss << stepName << ',' << skippedInt << "\n";
                lines.push_back(oss.str());
            }
            
        }

        file.close();
        std::ofstream outputFile("skipped_steps.csv");
        if(outputFile.is_open())
        {
            for(const auto& updatedLine: lines)
                outputFile << updatedLine;
            outputFile.close();
        }
        else std::cerr << "Error opening output file 'skipped_steps.csv'\n";
    }else std::cerr << "Error opening file 'skipped_steps.csv'\n";
}

//functie pentru crearea unui nou flow
void NewFlowCreation(string name_of_flow)
{
    string answer; //variabila pentru raspunsul in cazul utilizarii sau nu a unui pas
    FlowCreation myflow(name_of_flow); //crearea unui obiect pentru clasa de crearea
    int numberInput_present = 0; //prezenta pasului NumberInputStep pentru a se putea folosi si CalculusInputStep
    int ok1 = 0, ok2 = 0, ok3 = 0, ok4 = 0, ok5 = 0, ok6 = 0, ok7 = 0, ok8 = 0, ok9 = 0, ok10 = 0; //contori ce semnficia folosirea sau nu a unui pas
    float numberInput_copy; //copiaza valoarea introdusa de utlizator la NumberInputStep
    for(int i = 0; i < 10; i++)
    {
        //la i = 0 sunt incluse explicatiile pentru logica abordata
        if(i == 0)
        {
            std::cout << "Do you want to add Title Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                string new_title, new_subtitle;
                std::cout << "Title: ";
                std::getline(cin, new_title);

                std::cout << "Subtitle: ";
                std::getline(cin, new_subtitle);

                //crearea unui nou obiect pentru flow 
                TitleStep* titleStep = new TitleStep(new_title, new_subtitle);
                myflow.add_steps(titleStep); //adaugarea pasului in vector
                myflow.execute_step(titleStep); //executarea pasului pentru ca utilizatorul sa vada pasul complet
                ok1 = 1; //marcheaza utilizarea pasului curent
                i--; //pentru a putea introduce de mai multe ori acelasi tip de pas 
            }
            else if(ok1 == 0) skipped_steps("TitleStep"); //apelarea functiei pentru fiecarea pas sarit
        }
        else if(i == 1)
        {
            std::cout << "Do you want to add Text Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                string new_title, new_copy;
                std::cout << "Title: ";
                std::getline(cin, new_title);

                std::cout << "Copy: ";
                std::getline(cin, new_copy);

                TextStep* textStep = new TextStep(new_title, new_copy);
                myflow.add_steps(textStep);
                myflow.execute_step(textStep);
                ok2 = 1;
                i--;
            }
            else if(ok2 == 0) skipped_steps("TextStep");
        }
        else if(i == 2)
        {
            std::cout << "Do you want to add Text Input Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                string new_description;
                std::cout << "Description: ";
                std::getline(cin, new_description);

                TextInputStep* textInputStep = new TextInputStep(new_description);
                myflow.add_steps(textInputStep);
                myflow.execute_step_newFlow(textInputStep);
                ok3 = 1;
                i--;
            }
            else if(ok3 == 0) skipped_steps("TextInputStep");
        }
        else if(i == 3)
        {
            std::cout << "Do you want to add Number Input Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                string new_description;
                numberInput_present = 1;

                std::cout << "Description: ";
                std::getline(cin, new_description);

                NumberInputStep* numberInputStep = new NumberInputStep(new_description);
                myflow.add_steps(numberInputStep);
                myflow.execute_step_newFlow(numberInputStep);
                ok4 = 1;
                i--;
            }
            else if(ok4 == 0) skipped_steps("NumberInputStep");
        }
        else if(i == 4 && numberInput_present == 1) //se poate adauga CalculusStep doar daca s-a introdus si NumberInputStep
        {
            std::cout << "Do you want to add Calculus Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                int new_steps;
                string new_operations;
                int ok = 0;
                int ok_for_operation = 0;

                while(ok_for_operation == 0)
                {
                    std::cout << "Operation: ";
                    std::getline(cin, new_operations);
                    ok = 0;
                    try
                    {
                        for(const auto& op: operations_clc) //cautarea operatiei introudse daca exista sau nu
                            if(op == new_operations)
                                ok = 1;
                        if(ok == 0)
                            throw std::runtime_error("The operation is not defined!\n"); //eroare in cazul introducerii gresite a operatiei
                        ok_for_operation = 1;
                    }
                    catch(const std::exception& e)
                    {
                        error_completedFlow++;
                        errors_steps("CalculusStep"); //actualizarea fisierului cu erorile pentru pasi
                        std::cerr << "Error! " << e.what() << '\n';
                        std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                        std::cerr << "Answer: ";
                        std::getline(cin, answer); //posibilitatea de a incerca din nou
                        if(answer == "yes")
                        {
                            ok_for_operation = 0;
                        }
                        else ok_for_operation = 1;
                    }
                }
              
                CalculusStep* calculusStep = new CalculusStep(new_operations);
                myflow.add_steps(calculusStep);
                myflow.execute_step_newFlow(calculusStep);
                ok5 = 1;
                i--;
            }
            else if(ok5 == 0) skipped_steps("CalculusStep");
        }
        else if(i == 5)
        {
            std::cout << "Do you want to add Text File Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                string new_description;
                std::cout << "Description: ";
                std::getline(cin, new_description);

                TextFileStep* textFileStep = new TextFileStep(new_description);
                myflow.add_steps(textFileStep);
                myflow.execute_step_newFlow(textFileStep);
                ok6 = 1;
                i--;
            }
            else if(ok6 == 0) skipped_steps("TextFileStep");
        }
        else if(i == 6)
        {
            std::cout << "Do you want to add CSV File Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                string new_description;
                std::cout << "Description: ";
                std::getline(cin, new_description);

                CSVFileStep* csvFileStep = new CSVFileStep(new_description);
                myflow.add_steps(csvFileStep);
                myflow.execute_step_newFlow(csvFileStep);
                ok7 = 1;
                i--;
            } 
            else if(ok7 == 0) skipped_steps("CSVFileStep");
        }
        else if(i == 7 && (txt_present == 1 || csv_present == 1)) //se poate adauga pasul de Display doar daca s-a adaugat
        {                                                         //cel putin un pas de TextFileStep sau CSVFileStep
            std::cout << "Do you want to add Display Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                int new_step;
                int ok = 0;
                DisplayStep* displayStep = new DisplayStep(new_step);
                myflow.add_steps(displayStep);
                myflow.execute_step_newFlow(displayStep);
                ok8 = 1;
                i--;
            }
            else if(ok8 == 0) skipped_steps("DisplayStep");
        }
        else if(i == 8)
        {
            std::cout << "Do you want to add Output Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                int new_step;
                string new_filename, new_title, new_description;
                int ok = 0;
                while(ok == 0)
                {
                    std::cout << "Description: ";
                    std::getline(cin, new_description);
                    try
                    {
                        std::cout << "Step: ";
                        if (!(cin >> new_step)) {
                            throw runtime_error("Invalid input. Not a number."); //verificare daca s-a introdus un numar pentru pasul ce trebuie afisat
                        }
                        
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "File Name (introduce a txt file): ";
                        std::getline(cin, new_filename);

                        
                        std::cout << "Title: ";
                        std::getline(cin, new_title);

                        OutputStep* outputstep = new OutputStep(new_step, new_filename, new_title, new_description);
                        myflow.add_steps(outputstep);
                        myflow.execute_step(outputstep);
                        ok = 1;
                    }
                    catch(const std::exception& e)
                    {
                        error_completedFlow++;
                        errors_steps("OutputStep"); //actualizarea numarului de erori
                        std::cerr << e.what() << '\n';
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                        std::cerr << "Answer: ";
                        std::getline(cin, answer);
                        if(answer == "yes")
                            ok = 0;
                        else ok = 1;
                    }

                }
                ok9 = 1;
                i--;
            }
            else if(ok9 == 0) skipped_steps("OutputStep");
        }
        else if(i == 9)
        {
            std::cout << "Do you want to add End Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            if(answer == "yes")
            {
                EndStep* endStep = new EndStep();
                myflow.add_steps(endStep);
                myflow.execute_step(endStep);
                ok10 = 1;
            }
            else if(ok10 == 0) skipped_steps("EndStep");    
        }
    }

    cout << "\n\nThe steps for the " << name_of_flow << " flow are:\n";
    myflow.display_all_steps_newFlow(); //afisrea pe ecran a pasilor
    myflow.add_to_csv(); //adaugarea flow-ului in fisierul cu date esentiale
    myflow.add_to_csv_expanded(); //aduagarea flow-ului in fisierul ce contine toate datele despre flow
    errors_completed_flows(name_of_flow); //aduagrea flow-ului la fisierul pentru erori
}

//functie pentru afisrea tuturor flow-urilor disponibile
void show_flows() 
{
    std::ifstream file("flows.csv");

    if (file.is_open()) 
    {
        std::cout << "These are the available flows with the specific steps:\n";
        std::cout << "Name of the flow | Steps | Date of creation | The number of uses\n\n";
        std::string line;
        while (std::getline(file, line)) 
        {
            // Utilizarea stringstream pentru a separa valorile CSV
            std::istringstream iss(line);
            std::string value;

            while (std::getline(iss, value, ',')) 
            {
                std::cout << value << " | ";
            }

            std::cout << "\n\n";
        }
        //posibilitatea de a merge inapoi la meniul principals
        std::cout << "Press 'b' if you want to go back to the Main Menu or press <enter> to continue\nAnswer: ";
        string character;
        std::getline(cin, character);
        if(character == "b")
            loop = 0;
        else loop = 1; 
        file.close();
    } 
    else std::cerr << "Error opening file 'flows.csv'\n";
}

//functie pentru citirea si parcurgerea unui flow
void read_flows(string name_of_flow)
{
    FlowCreation myflow(name_of_flow);

    int verify = 0, index = 0;
    float numberInput_copy;
    string answer_loop;

    while(verify == 0)
    {
        int ok = 0 , double_display = 0; //aflare tip de pas si evitarea afisarii de doua ori a tipului

        // variabile de verificare pentru retinerea detaliilor definite la crearea step-ului
        int ok_title_title = 0, ok_title_subtitle = 0, ok_title_text = 0, ok_copy_text = 0, ok_for_calculus = 0;

        int ok_correct_display = 0; //pentru a retine bine datele pentru obiecte, separa datele pentru a fi retinute corect
        int no_input_step = 0; //pentru verificare utilizare number input step

        //contori pentru crearea corecta unui OutputStep
        int ok_step_out = 0, ok_fn_out = 0, ok_title_out = 0, ok_description_out = 0;

        //varibile pentru retinerea titlului si subtitlului de la TitleStep
        string title_ts, subtitle_ts;

        //variabile pentru retinerea titlului si copy-ului de la TextStep
        string title_texts, copy_texts;

        //variabile pentru retinerea descrierii si a input-ului de la utilizator pentru TextInputStep
        string description_ti, text_input_ti;

        //variabile pentru a retine descrieriile pentru NumberInputStep, TextFileStep si CSVFileStep
        string description_ni, description_tf, description_csvf;

        //variabile pentru retinerea numelor fisierelor txt/csv introduse
        string file_name, file_namecsv;

        //variabila pentru numarul introdus de utilizator la NumberInputStep
        float number_input;

        //variabile pentru retinerea datelor de la OutputStep
        string step_output_str, fn_out, title_out, description_out;
        int step_output;

        string answer_for_step; //pentru folosirea sau nu a unui pas din flow

        std::ifstream file_expanded("flows_expanded.csv"); //lucram cu acest fisier pentru a extrage toate datele existente in flow

        if(index == 1) //pentru cazul in care a fost introdus un flowname gresit
        {
            std::cout << "What flow do you want tu use?(Enter the name of the flow)\nAnswer: ";
            std::getline(cin, name_of_flow);
        }

        if(file_expanded.is_open())
        {
            std::string line;
            while(std::getline(file_expanded, line))
            {
                std::istringstream iss(line);
                std::string flowName, data;

                if(std::getline(iss, flowName, ','))
                {
                    if(flowName == name_of_flow)
                    {
                        verify = 1; //gasirea flow-ului cautat
                        started_flow = 1; //marcheaza ca flow-ul a fost selectat si inceput
                        while(std::getline(iss, data, ','))
                        {
                            double_display = 0; //reinitializare pentru afisarea corecta
                            if(data == "TitleStep")
                            {
                                std::cout << "Do you want to add Title Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 1; //marcarea gasirii tipului de pas
                                    double_display = 1; //pentru a evita sa se afisese "TitleStep" si "Title Step" si sa ramana doar a doua varianta
                                }
                                else
                                {
                                    ok = 0;
                                    completed_flow = 0; //inseamna ca pasul a fost sarit si flow-ul nu poate fi considerat complet
                                    skipped_steps("TitleStep");
                                }
                            }
                            else if(data == "TextStep")
                            {
                                std::cout << "Do you want to add Text Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 2;
                                    double_display = 1;
                                }
                                else
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("TextStep");
                                }
                            }
                            else if(data == "TextInputStep")
                            {
                                std::cout << "Do you want to add Text Input Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 3;
                                    double_display = 1;
                                }
                                else 
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("TextInputStep");
                                }
                            }
                            else if(data == "NumberInputStep")
                            {
                                std::cout << "Do you want to add Number Input Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 4;
                                    no_input_step = 1; //inseamna ca step-ul este folosit si astfel se poate folosi si CalculusStep
                                    double_display = 1;
                                }
                                else
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("NumberInputStep");
                                }
                            }
                            else if(data == "CalculusStep")
                            {
                                if(no_input_step == 0) //semnifica utilizarea pasului de NumberInputStep
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                }
                                else
                                {
                                    std::cin.ignore();
                                    std::cout << "Do you want to add Calculus Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                    std::cout << "Answer: ";
                                    std::getline(cin, answer_for_step);
                                    if(answer_for_step == "yes")
                                    {
                                        ok = 5;
                                        double_display = 1;
                                    }
                                    else
                                    {
                                        ok = 0;
                                        completed_flow = 0;
                                        skipped_steps("CalculusStep");
                                    }
                                }
                            }
                            else if(data == "DisplayStep")
                            {
                                if(txt_present == 1 || csv_present == 1)
                                {
                                    std::cout << "Do you want to add Display Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                    std::cout << "Answer: ";
                                    std::getline(cin, answer_for_step);
                                    if(answer_for_step == "yes")
                                    {
                                        ok = 8;
                                        double_display = 1;
                                    }
                                    else
                                    {
                                        ok = 0;
                                        completed_flow = 0;
                                    }
                                }
                                else
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("DisplayStep");
                                }
                            }
                            else if(data == "TextFileStep")
                            {
                                std::cin.ignore();
                                std::cout << "Do you want to add Text File Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 6;
                                    double_display = 0;
                                }
                                else 
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("TextFileStep");
                                }
                            }
                            else if(data == "CSVFileStep")
                            {
                                std::cout << "Do you want to add CSV File Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 7;
                                    double_display = 0;
                                }
                                else 
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("CSVFileStep");
                                }
                            }
                            else if(data == "OutputStep")
                            {
                                std::cout << "Do you want to add Output Step?(Write <yes> to use this step or press <enter> for skipping this step)\n";
                                std::cout << "Answer: ";
                                std::getline(cin, answer_for_step);
                                if(answer_for_step == "yes")
                                {
                                    ok = 9;
                                    double_display = 1;
                                }
                                else 
                                {
                                    ok = 0;
                                    completed_flow = 0;
                                    skipped_steps("OutputStep");
                                }
                            }
                            else if(data == "EndStep")
                            {
                                ok = 10;
                                double_display = 1;
                            }

                            //explicatiile pentru logica de parcurgere sunt continute la ok == 1
                            if(ok == 1 && double_display == 0)
                            {
                                if(ok_correct_display == 0) //inseamna ca 'data' contine "Title Step"
                                {
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    if(ok_title_title == 0) //pentru retinerea primei componente a obiectului
                                    {
                                        size_t pos = data.find("Title: "); //selectarea a tot ce este dupa "Title: "
                                        if (pos != std::string::npos)
                                            title_ts = data.substr(pos + std::string("Title: ").length());   
                                        ok_title_title = 1; //marcheaza ca s-a gasit titlul
                                    }
                                    else if(ok_title_subtitle == 0)
                                    {
                                        size_t pos = data.find("Subtitle: ");
                                        if (pos != std::string::npos)
                                            subtitle_ts = data.substr(pos + std::string("Subtitle: ").length());  
                                        ok_title_subtitle = 1; //marcheaza ca s-a gasit subtitlul
                                    }
                                    if(ok_title_title == 1 && ok_title_subtitle == 1) //dupa ce au fost retinute title si subtitle se poate crea corect obiectul
                                    {
                                        TitleStep* titleStep = new TitleStep(title_ts, subtitle_ts); //se creaza obiectul
                                        myflow.add_steps(titleStep); //se adauga pasul
                                        myflow.execute_step(titleStep); //se executa pasul
                                        ok = 0; //resetam variabilele pentru trecerea la urmatorul pas
                                        ok_title_title = 0;
                                        ok_title_subtitle = 0;
                                        ok_correct_display = 0;
                                        std::cout << '\n';
                                    }
                                }
                            }
                            else if(ok == 2 && double_display == 0)
                            {
                                if(ok_correct_display == 0)
                                {
                                    ok_correct_display = 1;
                                }
                                else 
                                {
                                    if(ok_title_text == 0)
                                    {
                                        size_t pos = data.find("Title: ");
                                        if (pos != std::string::npos)
                                            title_texts = data.substr(pos + std::string("Title: ").length()); 
                                        
                                        ok_title_text = 1;
                                    }
                                    else if(ok_copy_text == 0)
                                    {
                                        size_t pos = data.find("Copy: ");
                                        if (pos != std::string::npos)
                                            copy_texts = data.substr(pos + std::string("Copy: ").length()); 
                                        
                                        ok_copy_text = 1;
                                    }
                                    if(ok_title_text == 1 && ok_copy_text == 1)
                                    {
                                        TextStep* textStep = new TextStep(title_texts, copy_texts);
                                        myflow.add_steps(textStep);
                                        myflow.execute_step(textStep);
                                        ok = 0;
                                        ok_title_text = 0;
                                        ok_copy_text = 0;
                                        ok_correct_display = 0;
                                        std::cout << '\n';
                                    }
                                }
                            }
                            else if(ok == 3 && double_display == 0)
                            {
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    std::cout << data << '\n';
                                    size_t pos = data.find("Description: ");
                                        if (pos != std::string::npos)
                                            description_ti = data.substr(pos + std::string("Description: ").length());
                                    std::cin.clear(); // Reseteaza orice erori de intrare
                                    std::cout << "Text Input: ";
                                    std::getline(cin, text_input_ti); //utilizatorul introduce

                                    TextInputStep* textInputStep = new TextInputStep(description_ti, text_input_ti);
                                    myflow.add_steps(textInputStep);
                                    std::cout << "\n--->After entering the datas:";
                                    myflow.execute_step(textInputStep);
                                    ok = 0;
                                    ok_correct_display = 0;
                                    std::cout << '\n';
                                }
                            }
                            else if(ok == 4 && double_display == 0)
                            {
                                string answer;
                                
                                int rule = 0; //variabila pentru bucla in cazul introducerii gresite
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    std::cout << data << '\n';
                                    description_ni = data;
                                    
                                    while(rule == 0)
                                    {
                                        std::cout << "Number Input: ";
                                        try
                                        {
                                            if (!(cin >> number_input)) {
                                                throw runtime_error("\n!Invalid input. Not a number!\n"); //cand nu este introdus un numar
                                            }
                                            NumberInputStep* numberInputStep = new NumberInputStep(description_ni, number_input);
                                            myflow.add_steps(numberInputStep);
                                            std::cout << "\n--->After entering the datas:";
                                            myflow.execute_step(numberInputStep);
                                            rule = 1;
                                            ok = 0;
                                            ok_correct_display = 0;
                                            std::cout << '\n';
                                        }
                                        catch(const std::exception& e)
                                        {
                                            std::cerr << e.what() << '\n';
                                            errors_steps("NumberInputStep");
                                            error_completedFlow++;
                                            std::cin.clear();
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                                            std::cerr << "Answer: ";
                                            std::getline(cin, answer);
                                            if(answer == "yes")
                                                rule = 0;
                                            else rule = 1;
                                        }
                                        
                                    }
                                }    
                            }
                            else if(ok == 5 && double_display == 0)
                            {
                                string operation, answer;
                                int step_clc, rule = 0;
                                float result = 0;
                                if(ok_correct_display == 0)
                                {
                                    std::cout << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    std::cout << data << '\n';
                                    size_t pos = data.find("Operation: ");
                                    if (pos != std::string::npos)
                                    {
                                        std::string op = data.substr(pos + std::string("Operation: ").length());
                                        for (const std::string& op_clc : operations_clc)
                                            if(op == op_clc)
                                            {
                                                operation = op;
                                                break;
                                            }
                                    }
                                    
                                    while(rule == 0)
                                    {
                                        std::cin.clear();
                                        std::cout << "Step: ";
                                        try
                                        {
                                            
                                            if (!(cin >> step_clc)) 
                                                throw runtime_error("\n!Invalid input. Not a number!\n"); //cand nu este introdus un numar
                                            ok_for_calculus = 1;
                                            rule = 1;
                                        }
                                        catch(const std::exception& e)
                                        {
                                            errors_steps("CalculusStep");
                                            error_completedFlow++;
                                            std::cerr << e.what() << '\n';
                                            std::cin.clear();
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                                            std::cerr << "Answer: ";
                                            std::getline(cin, answer);
                                            if(answer == "yes")
                                                rule = 0;
                                            else rule = 1;
                                        }
                                    }
                                    if(ok_for_calculus == 1)
                                    {
                                        result = operations_for_calculus<float, int>(number_input, step_clc, operation); //apelarea functiei pentru calcul
                                        CalculusStep* calculusStep = new CalculusStep(step_clc, operation, result);
                                        myflow.add_steps(calculusStep);
                                        std::cout << "\n--->After entering the datas:";
                                        myflow.execute_step(calculusStep);
                                        ok = 0;
                                        rule = 1;
                                        ok_correct_display = 0;
                                        std::cout << '\n';
                                    }
                                }
                            }
                            else if(ok == 6 && double_display == 0) //contine detaliile pentru adaugarea fisierelor txt si csv
                            {
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    std::cout << data << '\n';
                                    description_tf = data;
                                    std::cout << "File name: ";
                                    std::getline(cin, file_name); //citirea fisierului
                                    TextFileStep* textFileStep = new TextFileStep(description_tf, file_name);
                                    myflow.execute_step(textFileStep);
                                    if(txt_present == 1)
                                         myflow.add_steps(textFileStep); //adaugarea pasului doar daca fisierul introdus exista
                                    ok = 0;
                                    ok_correct_display = 0;
                                    txt_present = 1;
                                    std::cout << '\n';
                                }
                            }
                            else if(ok == 7 && double_display == 0)
                            {
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    std::cout << data << '\n';
                                    description_csvf = data;
                                    std::cout << "File name: ";
                                    std::getline(cin, file_namecsv);
                                    CSVFileStep* textFileStep = new CSVFileStep(description_csvf, file_namecsv);
                                    myflow.execute_step(textFileStep);
                                    if(csv_present == 1)
                                        myflow.add_steps(textFileStep);
                                    ok = 0;
                                    ok_correct_display = 0;
                                    csv_present = 1;
                                    std::cout << '\n';
                                }
                            }
                            else if(ok == 8 && double_display == 0)
                            {
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    std::cout << data << '\n';
                                    int rule = 0;
                                    int new_step;
                                    string answer;
                                    while(rule == 0)
                                    {
                                        try
                                        {
                                            std::cout << "Introduce 1 for txt file or 2 for csv file\nStep: ";
                                            if (!(cin >> new_step)) 
                                                throw runtime_error("Invalid input. Not a number.");
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            DisplayStep* displayStep = new DisplayStep(new_step);
                                            myflow.add_steps(displayStep);
                                            myflow.execute_step(displayStep);
                                            ok = 0;
                                            ok_correct_display = 0;
                                            rule = 1;
                                            std::cout << '\n';
                                        }
                                        catch(const std::exception& e)
                                        {
                                            errors_steps("DisplayStep");
                                            error_completedFlow++;
                                            std::cerr << e.what() << '\n';
                                            std::cin.clear();
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            std::cerr << "Do you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                                            std::cerr << "Answer: ";
                                            if(answer == "yes")
                                                rule = 0;
                                            else rule = 1;
                                        }
                                        
                                    }
                                }
                            }
                            else if(ok == 9 && double_display == 0)
                            {
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                }
                                else
                                {
                                    if(ok_step_out == 0)
                                    {
                                        size_t pos = data.find("Step: ");
                                            if (pos != std::string::npos)
                                                step_output_str = data.substr(pos + std::string("Step: ").length());
                                        step_output = std::stoi(step_output_str);
                                        ok_step_out = 1;
                                    }
                                    else if(ok_fn_out == 0)
                                    {
                                        size_t pos = data.find("File name: ");
                                        if (pos != std::string::npos)
                                            fn_out = data.substr(pos + std::string("File name: ").length());
                                        ok_fn_out = 1;
                                    }
                                    else if(ok_title_out == 0)
                                    {
                                        size_t pos = data.find("Title: ");
                                        if (pos != std::string::npos)
                                            title_out = data.substr(pos + std::string("Title: ").length());
                                        ok_title_out = 1;
                                    }
                                    else if(ok_description_out == 0)
                                    {
                                        size_t pos = data.find("Description: ");
                                        if (pos != std::string::npos)
                                            description_out = data.substr(pos + std::string("Description: ").length());
                                        ok_description_out = 1;
                                    }
                                    if(ok_step_out == 1 && ok_fn_out == 1 && ok_title_out == 1 && ok_description_out == 1)
                                    {
                                        OutputStep* outputStep = new OutputStep(step_output, fn_out, title_out, description_out);
                                        myflow.add_steps(outputStep);
                                        myflow.execute_step(outputStep);
                                        ok = 0;
                                        ok_correct_display = 0;
                                        std::cout << '\n';
                                    }
                                }
                            }
                            else if(ok == 10 && double_display == 0)
                            {
                                if(ok_correct_display == 0)
                                {
                                    std::cout << '\n' << data << '\n';
                                    ok_correct_display = 1;
                                    EndStep* endStep = new EndStep();
                                    myflow.add_steps(endStep);
                                    myflow.execute_step(endStep);
                                }
                            }
                        }
                    }
                }
            }
            file_expanded.close();
        }
        if(verify == 0)
        {
            std::cout << "\n!The flow doesn't exist!\nDo you want to try again?(Write <yes> if you want to continue or just press <enter> for skipping)\nAnswer: ";
            std::getline(cin, answer_loop); //posibilitatea de a introduce un flow existent
            if(answer_loop == "yes")
            {
                verify = 0;
                index = 1;
            }
            else verify = 1;
        }
        else
        {
            std::cout << "\n\nThe completed steps for the " << name_of_flow << " flow are:\n";
            myflow.display_all_steps();
        }
    }


}

//functie pentru actualizarea frecventei de utilizare a flow-urilor
void update_flows(const std::string& name_of_flow) 
{
    // Deschidere fisier in modul de citire
    std::ifstream inputFile("flows.csv");
    // Creare fisier temporar in modul de scriere
    std::ofstream tempFile("temp_flows.csv");

    if (inputFile.is_open() && tempFile.is_open()) 
    {
        std::string line;
        while (std::getline(inputFile, line)) 
        {
            std::string flow;
            std::istringstream iss(line);
            std::getline(iss, flow, ',');

            // Verifica daca prima coloana corespunde cu name_of_flow
            if (flow == name_of_flow) 
            {
                std::string temp;
                std::vector<std::string> values;
                values.push_back(name_of_flow);
                // Descompunere linie in valori separate
                while (std::getline(iss, temp, ',')) 
                {
                    values.push_back(temp);
                }

                // Incrementarea ultimei valori de pe linie cu 1
                if (!values.empty()) 
                {
                    int lastValue = std::stoi(values.back());
                    lastValue++;

                    // Actualizare vector
                    values.back() = std::to_string(lastValue);
                }

                // Reasamblare linie si scriere in fisierul temporar
                tempFile << values[0];

                for (size_t i = 1; i < values.size(); ++i) 
                {
                    tempFile << ',' << values[i];
                }

                tempFile << std::endl; // Trecere la următoarea linie
            } 
            else 
            {
                // Daca nu este linia cautata, se scrie in fisierul temporar asa cum este
                tempFile << line << std::endl;
            }
        }

        // Se inchid fisierele
        inputFile.close();
        tempFile.close();

        // Se copiaza continutul fisierului temporar in cel original
        std::ifstream tempInputFile("temp_flows.csv");
        std::ofstream outputFile("flows.csv", std::ios::trunc); // deschide cu std::ios::trunc pentru a sterge continutul existent
        outputFile << tempInputFile.rdbuf();

        // Afiseaza pe ecran continutul
        std::cout << tempInputFile.rdbuf();

        // Inchidere si stergere fisier temporar
        tempInputFile.close();
        remove("temp_flows.csv");
    } 
    else 
    {
        std::cerr << "Error opening files for updating flows\n";
    }
}

//functie pentru actualizarea detaliilor inceperii si finalizarii complete a unui flow
void flows_started_completed(const std::string& name_of_flow)
{
    std::ifstream file("Flows_categories.csv");
    if(file.is_open())
    {
        std::string line;
        std::vector<std::string> lines;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string flowName, startedValue, completedValue;

            if(std::getline(iss, flowName, ',') && std::getline(iss, startedValue, ',') && std:: getline(iss, completedValue, ','))
            {
                int startedInt = std::stoi(startedValue);
                int completedInt = std::stoi(completedValue);
                if(flowName == name_of_flow)
                {
                    if(started_flow == 1) //inseamna ca flow-ul a fost inceput
                        startedInt++;
                    if(completed_flow == 1) //inseamna ca flow-ul a fot parcurs in totalitate
                    {
                        completedInt++;
                        errors_completed_flows(flowName); //adaugarea numarului de erori pentru flow complet
                    }
                }
                std::ostringstream oss;
                oss << flowName << ',' << startedInt << ',' << completedInt << '\n';
                lines.push_back(oss.str());
            }
        }

        file.close();

        std::ofstream outputFile("Flows_categories.csv");
        if(outputFile.is_open())
        {
            for(const auto& updatedLine: lines)
                outputFile << updatedLine;
            outputFile.close();
        }
        else std::cerr << "Error opening output file 'Flows_categories.csv'\n";
    }
    else std::cerr<< "Error opening file 'Flows_categories.csv'\n";
}

//functie pentru stergerea unui flow
void delete_flow(const std::string& flowName) 
{
    // 1.Sterge din flows.csv
    std::ifstream inputFileFlows("flows.csv");
    if (inputFileFlows.is_open()) 
    {
        std::vector<std::string> linesFlows;
        std::string lineFlows;
        while (std::getline(inputFileFlows, lineFlows)) 
        {
            std::istringstream iss(lineFlows);
            std::string currentFlowName;
            if (std::getline(iss, currentFlowName, ',') && currentFlowName != flowName) 
            {
                linesFlows.push_back(lineFlows);
            }
        }
        inputFileFlows.close();

        std::ofstream outputFileFlows("flows.csv");
        if (outputFileFlows.is_open()) 
        {
            for (const auto& updatedLine : linesFlows) 
            {
                outputFileFlows << updatedLine << '\n';
            }
            outputFileFlows.close();
        }
        else 
        {
            std::cerr << "Error opening output file 'flows.csv'\n";
        }
    }
    else 
    {
        std::cerr << "Error opening file 'flows.csv'\n";
    }

    // 2.Sterge din flows_categories.csv
    std::ifstream inputFileCategories("flows_categories.csv");
    if (inputFileCategories.is_open()) 
    {
        std::vector<std::string> linesCategories;
        std::string lineCategories;
        while (std::getline(inputFileCategories, lineCategories)) 
        {
            std::istringstream iss(lineCategories);
            std::string currentFlowName;
            if (std::getline(iss, currentFlowName, ',') && currentFlowName != flowName) 
            {
                linesCategories.push_back(lineCategories);
            }
        }
        inputFileCategories.close();

        std::ofstream outputFileCategories("flows_categories.csv");
        if (outputFileCategories.is_open()) 
        {
            for (const auto& updatedLine : linesCategories) 
            {
                outputFileCategories << updatedLine << '\n';
            }
            outputFileCategories.close();
        }
        else 
        {
            std::cerr << "Error opening output file 'flows_categories.csv'\n";
        }
    }
    else 
    {
        std::cerr << "Error opening file 'flows_categories.csv'\n";
    }

    // 3.Sterge din errors_completed_flows.csv
    std::ifstream inputFileErrors("errors_completed_flows.csv");
    if (inputFileErrors.is_open()) 
    {
        std::vector<std::string> linesErrors;
        std::string lineErrors;
        while (std::getline(inputFileErrors, lineErrors)) 
        {
            std::istringstream iss(lineErrors);
            std::string currentFlowName;
            if (std::getline(iss, currentFlowName, ',') && currentFlowName != flowName) 
            {
                linesErrors.push_back(lineErrors);
            }
        }
        inputFileErrors.close();

        std::ofstream outputFileErrors("errors_completed_flows.csv");
        if (outputFileErrors.is_open()) 
        {
            for (const auto& updatedLine : linesErrors) 
            {
                outputFileErrors << updatedLine << '\n';
            }
            outputFileErrors.close();
        }
        else 
        {
            std::cerr << "Error opening output file 'errors_completed_flows.csv'\n";
        }
    }
    else 
    {
        std::cerr << "Error opening file 'errors_completed_flows.csv'\n";
    }

    // 4. Sterge din flows_expanded.csv
    std::ifstream inputExpandedFile("flows_expanded.csv");
    if (inputExpandedFile.is_open()) 
    {
        std::vector<std::string> linesExpanded;
        std::string lineExpanded;
        while(std::getline(inputExpandedFile, lineExpanded)) 
        {
            std::istringstream iss(lineExpanded);
            std::string currentFlowName;
            if (std::getline(iss, currentFlowName, ',') && currentFlowName != flowName) 
            {
                linesExpanded.push_back(lineExpanded);
            }
        }
        inputFileErrors.close();

        std::ofstream outputExpandedFile("flows_expanded.csv");
        if (outputExpandedFile.is_open()) 
        {
            for (const auto& updatedLine : linesExpanded) 
            {
                outputExpandedFile << updatedLine << '\n';
            }
            outputExpandedFile.close();
        }
        else 
        {
            std::cerr << "Error opening output file 'errors_completed_flows.csv'\n";
        }
    }
    else 
    {
        std::cerr << "Error opening file 'errors_completed_flows.csv'\n";
    }
}

int main()
{
    while(loop == 0)
    {
        //mesaje introductive de ghidare a utilizatorului
        string message;
        std::cout << "This is an app for your processes! Use it to be more productive!\n";
        std::cout << "How do you want to start?\n";
        std::cout << "-->Press 1.New Flow\n-->Press 2.Use an existing flow\n-->Press 3.Info about csv files\n-->Press 4.Delete flow\n-->Press q.Quit app\nAnswer: ";
        getline(cin, message);

        if(message == "1")
        {
            string answer;
            std::cout << "These are all the steps you can use for your flow:\n";
            std::cout << "-->Title Step\n-->Text Step\n-->Text Input Step\n-->Number Input Step\n";
            std::cout << "-->Calculus Step\n-->Display Step\n-->Text File Step\n-->CSV File Step\n-->Output Step\n-->End Step\n";
            std::cout << "If you want details about any of these steps, just write the name of the step following by the <details> keywordor just press <enter> for skipping\n";
            std::cout << "Answer: ";
            std::getline(cin, answer);
            while(answer.empty() == false) 
            {
                descriptionOfSteps(answer);
                std::cout << "Do you want more details about steps?(If you want to know more, write again the name of the step following by the <details> keywordor just press enter for skipping)\n";
                std::cout << "Answer: ";
                std::getline(cin, answer);
            }

            std::cout << "Press 'b' if you want to go back to the Main Menu or press <enter> to continue\nAnswer: ";
            string character;
            std::getline(cin, character);
            if(character == "b")
                loop = 0;
            else loop = 1; 
            if(loop == 1)
            {
                
                std::cout << "Do you want to start to create a new flow?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                std::cout << "Answer: ";
                std::getline(cin, answer);
                if(answer == "yes")
                {
                    string name_of_flow;
                    std::cout << "Name of the flow: ";
                    std::getline(cin, name_of_flow);
                    NewFlowCreation(name_of_flow); //apelarea functiei pentru crearea unui flow nou
                }
            }
        }
        else if(message == "2")
        {
            string answer;
            show_flows(); //afisarea flow-uriloe existente
            if(loop == 1)
            {
                std::cout << "If you want details about any of these steps, just write the name of the step following by the <details> keywordor just press <enter> for skipping\n";
                std::cout << "Answer: ";
                std::getline(cin, answer);
                while(answer.empty() == false) 
                {
                    descriptionOfSteps(answer);
                    std::cout << "Do you want more details about steps?(If you want to know more, write again the name of the step following by the <details> keywordor just press enter for skipping)\n";
                    std::cout << "Answer: ";
                    std::getline(cin, answer);
                }

                std::cout << "Press 'b' if you want to go back to the Main Menu or press <enter> to continue\nAnswer: ";
                string character;
                std::getline(cin, character);
                if(character == "b")
                    loop = 0;
                else loop = 1; 
                if(loop == 1)
                {
                    std::cout << "Do you want to start to use a flow?(Write <yes> if you want to continue or just press <enter> for skipping)\n";
                    std::cout << "Answer: ";
                    std::getline(cin, answer);
                    if(answer == "yes")
                    {
                        std::cout << "What flow do you want tu use?(Enter the name of the flow)\nAnswer: ";
                        std::getline(cin, answer);
                        read_flows(answer); //apelarea functiei de citire si parcurgere
                        update_flows(answer); //update frecventa pentru flow-ul utilizat
                        flows_started_completed(answer); //update incepere-parcurgere_completa pentru flow-ul utilizat
                    }
                }
            }
        }
        else if(message == "3")
        {
            std::cout << "\nIf you want to find out the structure of a csv file, enter its name followed by the <details> keyword\n";
            std::cout << "Press enter to return to Main Menu\nAnswer: ";
            string answer_flows;
            std::getline(cin, answer_flows);
            while(answer_flows.empty() == false) 
            {
                descriptionOfFlowsFiles(answer_flows); //descrierea fisierlor csv cu analize
                std::cout << "Do you want more details about csv files?(If you want to know more, write again the flowname following by the <details> keyword or just press enter to return to Main Menu)\n";
                std::cout << "Answer: ";
                std::getline(cin, answer_flows);
            }
        }
        else if(message == "4")
        {
            std::cout << "\nWhich flow do you want to delete?\n";
            std::cout << "Press enter to return to Main Menu\nAnswer: ";
            string answer_delete;
            std::getline(cin, answer_delete);
             while(answer_delete.empty() == false) 
            {
                delete_flow(answer_delete); //apelarea functiei de stergere a flow-urilor
                std::cout << "Do you want to delete more flows?(If you want to delete more, write again the name of the flow or just press enter for skipping)\n";
                std::cout << "Answer: ";
                std::getline(cin, answer_delete);
            }

        }
        else if(message == "q") return 0 ; //iesirea directa din aplicatie
    }
    return 0;
}