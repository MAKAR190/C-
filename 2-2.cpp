#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <stdexcept>
#include <algorithm>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void processMathMLLine(const std::string& line, int lineNumber, std::stack<std::string>& tagStack) {
    std::string trimmedLine = trim(line);

    if (trimmedLine.empty()) return;

    size_t openTagsCount = 0;
    size_t closeTagsCount = 0;

    if (trimmedLine.find("<math") != std::string::npos) {
        openTagsCount++;
        tagStack.push("math");
    }
    if (trimmedLine.find("</math>") != std::string::npos) {
        closeTagsCount++;
        if (tagStack.empty() || tagStack.top() != "math") {
            throw std::runtime_error("Blad: Nieodpowiadajacy </math> w linijce " + std::to_string(lineNumber));
        }
        tagStack.pop();
    }

    if (trimmedLine.find("<mfenced>") != std::string::npos) {
        openTagsCount++;
        tagStack.push("mfenced");
    }
    if (trimmedLine.find("</mfenced>") != std::string::npos) {
        closeTagsCount++;
        if (tagStack.empty() || tagStack.top() != "mfenced") {
            throw std::runtime_error("Blad: Nieodpowiadajacy </mfenced> w linijce " + std::to_string(lineNumber));
        }
        tagStack.pop();
    }

    if (trimmedLine.find("<mrow>") != std::string::npos) {
        openTagsCount++;
        tagStack.push("mrow");
    }
    if (trimmedLine.find("</mrow>") != std::string::npos) {
        closeTagsCount++;
        if (tagStack.empty() || tagStack.top() != "mrow") {
            throw std::runtime_error("Blad: Nieodpowiadajacy </mrow> w linijce " + std::to_string(lineNumber));
        }
        tagStack.pop();
    }

    const char* tags[] = { "msup", "mi", "mo", "mn" };
    for (const char* tag : tags) {
        std::string openTag = "<" + std::string(tag) + ">";
        std::string closeTag = "</" + std::string(tag) + ">";

        if (trimmedLine.find(openTag) != std::string::npos) {
            openTagsCount++;
            tagStack.push(tag);
        }
        if (trimmedLine.find(closeTag) != std::string::npos) {
            closeTagsCount++;
            if (tagStack.empty() || tagStack.top() != tag) {
                throw std::runtime_error("Blad: Nieodpowiadajacy </" + std::string(tag) + "> w linijce " + std::to_string(lineNumber));
            }
            tagStack.pop();
        }
    }

    if (openTagsCount > 1 || closeTagsCount > 1) {
        throw std::runtime_error("Blad: Wiecej niz jeden tag w linijce " + std::to_string(lineNumber));
    }
}

void validateMathMLFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
    }

    std::string line;
    std::stack<std::string> tagStack;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;
        try {
            processMathMLLine(line, lineNumber, tagStack);
        }
        catch (const std::exception& e) {
            std::cerr << "Blad w linijce " << lineNumber << ": " << e.what() << std::endl;
            return;
        }
    }

    if (!tagStack.empty()) {
        std::cerr << "Blad: Pozostaly otwarte tagi w pliku." << std::endl;
        while (!tagStack.empty()) {
            std::cerr << "Nie zamkniety tag: <" << tagStack.top() << ">" << std::endl;
            tagStack.pop();
        }
    }
    else {
        std::cout << "Walidacja MathML zakonczona pomyslnie." << std::endl;
    }
}

int main() {
    try {
        validateMathMLFile("mathml_input.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Blad: " << e.what() << std::endl;
    }

    return 0;
}
std::string removeComments(const std::string& line) {
    size_t commentPos = line.find('%');
    if (commentPos != std::string::npos) {
        return line.substr(0, commentPos);
    }
    return line;
}
