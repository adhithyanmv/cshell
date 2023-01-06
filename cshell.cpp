#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>
using namespace std;

std::vector<std::string> command_history;

bool isInt(const std::string &s) {
  return std::all_of(s.begin(), s.end(), ::isdigit);
}

void showHistory(const vector<string>& history, const string& indexStr) {
    if (indexStr.empty()) {
        for (int i = 0; i < history.size(); i++) {
            cout << i + 1 << " " << history[i] << endl;
        }
    } else {
        int index;
        try {
            index = stoi(indexStr) - 1;
        } catch (const invalid_argument&) {
            cerr << "Error: invalid index" << endl;
            return;
        }

        if (index >= 0 && index < history.size()) {
            cout << history[index] << endl;
        } else {
            cerr << "Error: index out of range" << endl;
        }
    }
}

void cd(const string& path) {
    if(chdir(path.c_str()) < 0) {
        std::cerr << "Error changing directory to " << path << "\n";
    }
}

int execute(const string& cmd, const string& col) {
        FILE* pipe = popen((cmd + " 2>&1").c_str(), "r");

        if (!pipe) {
            std::cerr << "Error opening pipe" << std::endl;
            return -1;
        }

        char buffer[128];
        std::string result = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL) {
                cout << col << buffer;
            }
        }
        pclose(pipe);
        return 1;
}

int main() {

    std::map<std::string, std::string> aliases;
    aliases["-"] = "sudo";
    aliases["diff"] = "diff --color";
    aliases["egrep"] = "egrep --color=auto --exclude-dir={.bzr,CVS,.git,.hg,.svn,.idea,.tox}";
    aliases["fgrep"] = "fgrep --color=auto --exclude-dir={.bzr,CVS,.git,.hg,.svn,.idea,.tox}";
    aliases["g"] = "git";
    aliases["ga"] = "git add";
    aliases["gaa"] = "git add --all";
    aliases["gam"] = "git am";
    aliases["gama"] = "git am --abort";
    aliases["gamc"] = "git am --continue";
    aliases["gams"] = "git am --skip";
    aliases["gamscp"] = "git am --show-current-patch";
    aliases["gap"] = "git apply";
    aliases["gapa"] = "git add --patch";
    aliases["gapt"] = "git apply --3way";
    aliases["gau"] = "git add --update";
    aliases["gav"] = "git add --verbose";
    aliases["gb"] = "git branch";
    aliases["gbD"] = "git branch -D";
    aliases["gba"] = "git branch -a";
    aliases["gbd"] = "git branch -d";
    aliases["gbl"] = "git blame -b -w";
    aliases["gbnm"] = "git branch --no-merged";
    aliases["gbr"] = "git branch --remote";
    aliases["gbs"] = "git bisect";
    aliases["gbsb"] = "git bisect bad";
    aliases["gbsg"] = "git bisect good";
    aliases["gbsr"] = "git bisect reset";
    aliases["gbss"] = "git bisect start";
    aliases["gc"] = "git commit -v";
    aliases["c!"] = "git commit -v --amend";
    aliases["gca"] = "git commit -v -a";
    aliases["ca!"] = "git commit -v -a --amend";
    aliases["gcam"] = "git commit -a -m";
    aliases["can!"] = "git commit -v -a --no-edit --amend";
    aliases["cans!"] = "git commit -v -a -s --no-edit --amend";
    aliases["gcas"] = "git commit -a -s";
    aliases["gcasm"] = "git commit -a -s -m";
    aliases["gcb"] = "git checkout -b";
    aliases["gcd"] = "git checkout $(git_develop_branch)";
    aliases["gcf"] = "git config --list";
    aliases["gcl"] = "git clone --recurse-submodules";
    aliases["gclean"] = "git clean -id";
    aliases["gcm"] = "git checkout $(git_main_branch)";
    aliases["gcmsg"] = "git commit -m";
    aliases["cn!"] = "git commit -v --no-edit --amend";
    aliases["gco"] = "git checkout";
    aliases["gcor"] = "git checkout --recurse-submodules";
    aliases["gcount"] = "git shortlog -sn";
    aliases["gcp"] = "git cherry-pick";
    aliases["gcpa"] = "git cherry-pick --abort";
    aliases["gcpc"] = "git cherry-pick --continue";
    aliases["gcs"] = "git commit -S";
    aliases["gcsm"] = "git commit -s -m";
    aliases["gcss"] = "git commit -S -s";
    aliases["gcssm"] = "git commit -S -s -m";
    aliases["gd"] = "git diff";
    aliases["gdca"] = "git diff --cached";
    aliases["gdct"] = "git describe --tags $(git rev-list --tags --max-count=1)";
    aliases["gdcw"] = "git diff --cached --word-diff";
    aliases["gds"] = "git diff --staged";
    aliases["gdt"] = "git diff-tree --no-commit-id --name-only -r";
    aliases["gdup"] = "git diff @{upstream}";
    aliases["gdw"] = "git diff --word-diff";
    aliases["gf"] = "git fetch";
    aliases["gfa"] = "git fetch --all --prune --jobs=10";
    aliases["gfg"] = "git ls-files | grep";
    aliases["gfo"] = "git fetch origin";
    aliases["gg"] = "git gui citool";
    aliases["gga"] = "git gui citool --amend";
    aliases["ghh"] = "git help";
    aliases["gignore"] = "git update-index --assume-unchanged";
    aliases["gk"] = "gitk --all --branches &!";
    aliases["gke"] = "gitk --all $(git log -g --pretty=%h) &!";
    aliases["gl"] = "git pull";
    aliases["glg"] = "git log --stat";
    aliases["glgg"] = "git log --graph";
    aliases["glgga"] = "git log --graph --decorate --all";
    aliases["glgm"] = "git log --graph --max-count=10";
    aliases["glgp"] = "git log --stat -p";
    aliases["glo"] = "git log --oneline --decorate";
    aliases["globurl"] = "noglob urlglobber ";
    aliases["glods"] = "git log --graph --pretty='\''%Cred%h%Creset -%C(auto)%d%Creset %s %Cgreen(%ad) %C(bold blue)<%an>%Creset'\'' --date=short";
    aliases["glog"] = "git log --oneline --decorate --graph";
    aliases["gloga"] = "git log --oneline --decorate --graph --all";
    aliases["glola"] = "git log --graph --pretty='\''%Cred%h%Creset -%C(auto)%d%Creset %s %Cgreen(%ar) %C(bold blue)<%an>%Creset'\'' --all";
    aliases["glols"] = "git log --graph --pretty='\''%Cred%h%Creset -%C(auto)%d%Creset %s %Cgreen(%ar) %C(bold blue)<%an>%Creset'\'' --stat";
    aliases["glp"] = "_git_log_prettil";
    aliases["gluc"] = "git pull upstream $(git_current_branch)";
    aliases["glum"] = "git pull upstream $(git_main_branch)";
    aliases["gm"] = "git merge";
    aliases["gma"] = "git merge --abort";
    aliases["gmom"] = "git merge origin/$(git_main_branch)";
    aliases["gmtl"] = "git mergetool --no-prompt";
    aliases["gmtlvim"] = "git mergetool --no-prompt --tool=vimdiff";
    aliases["gmum"] = "git merge upstream/$(git_main_branch)";
    aliases["gp"] = "git push";
    aliases["gpd"] = "git push --dry-run";
    aliases["gpf"] = "git push --force-with-lease";
    aliases["gpf"] = "git push --force";
    aliases["gpoat"] = "git push origin --all && git push origin --tags";
    aliases["gpr"] = "git pull --rebase";
    aliases["gpristine"] = "git reset --hard && git clean -dffx";
    aliases["gpsup"] = "git push --set-upstream origin $(git_current_branch)";
    aliases["gpu"] = "git push upstream";
    aliases["gpv"] = "git push -v";
    aliases["gr"] = "git remote";
    aliases["gra"] = "git remote add";
    aliases["grb"] = "git rebase";
    aliases["grba"] = "git rebase --abort";
    aliases["grbc"] = "git rebase --continue";
    aliases["grbd"] = "git rebase $(git_develop_branch)";
    aliases["grbi"] = "git rebase -i";
    aliases["grbm"] = "git rebase $(git_main_branch)";
    aliases["grbo"] = "git rebase --onto";
    aliases["grbom"] = "git rebase origin/$(git_main_branch)";
    aliases["grbs"] = "git rebase --skip";
    aliases["grep"] = "grep --color=auto --exclude-dir={.bzr,CVS,.git,.hg,.svn,.idea,.tox}";
    aliases["grev"] = "git revert";
    aliases["grh"] = "git reset";
    aliases["grhh"] = "git reset --hard";
    aliases["grm"] = "git rm";
    aliases["grmc"] = "git rm --cached";
    aliases["grmv"] = "git remote rename";
    aliases["groh"] = "git reset origin/$(git_current_branch) --hard";
    aliases["grrm"] = "git remote remove";
    aliases["grs"] = "git restore";
    aliases["grset"] = "git remote set-url";
    aliases["grss"] = "git restore --source";
    aliases["grst"] = "git restore --staged";
    aliases["grt"] = "cd '$(git rev-parse --show-toplevel || echo .)'";
    aliases["gru"] = "git reset --";
    aliases["grup"] = "git remote update";
    aliases["grv"] = "git remote -v";
    aliases["gsb"] = "git status -sb";
    aliases["gsd"] = "git svn dcommit";
    aliases["gsh"] = "git show";
    aliases["gsi"] = "git submodule init";
    aliases["gsps"] = "git show --pretty=short --show-signature";
    aliases["gsr"] = "git svn rebase";
    aliases["gss"] = "git status -s";
    aliases["gst"] = "git status";
    aliases["gsta"] = "git stash push";
    aliases["gstaa"] = "git stash apply";
    aliases["gstall"] = "git stash --all";
    aliases["gstc"] = "git stash clear";
    aliases["gstd"] = "git stash drop";
    aliases["gstl"] = "git stash list";
    aliases["gstp"] = "git stash pop";
    aliases["gsts"] = "git stash show --text";
    aliases["gstu"] = "gsta --include-untracked";
    aliases["gsu"] = "git submodule update";
    aliases["gsw"] = "git switch";
    aliases["gswc"] = "git switch -c";
    aliases["gswd"] = "git switch $(git_develop_branch)";
    aliases["gswm"] = "git switch $(git_main_branch)";
    aliases["gts"] = "git tag -s";
    aliases["gtv"] = "git tag | sort -V";
    aliases["gunignore"] = "git update-index --no-assume-unchanged";
    aliases["gup"] = "git pull --rebase";
    aliases["gupa"] = "git pull --rebase --autostash";
    aliases["gupav"] = "git pull --rebase --autostash -v";
    aliases["gupom"] = "git pull --rebase origin $(git_main_branch)";
    aliases["gupomi"] = "git pull --rebase=interactive origin $(git_main_branch)";
    aliases["gupv"] = "git pull --rebase -v";
    aliases["gwch"] = "git whatchanged -p --abbrev-commit --pretty=medium";
    aliases["l"] = "ls -lah";
    aliases["la"] = "ls -lAh";
    aliases["ll"] = "ls -lh";
    aliases["ls"] = "ls --color=tty";
    aliases["lsa"] = "ls -lah";
    aliases["md"] = "mkdir -p";
    aliases["rd"] = "rmdir";

    while(true) {
        string col = "\033[0m";
        string input;

        cout << col << "cshell> " << "\033[0m";
        getline(cin, input);

        if(input.substr(0, 9) == "setcolor ") {
            string color = input.substr(9);
            if(color == "red" || color == "Red" || color == "RED") {
                col = "\033[38;2;255;0;0m";
                continue;
            } else if (color == "green" || color == "Green" || color == "GREEN") {
                col = "\033[38;2;0;255;0m";
                continue;
            } else if (color == "blue" || color == "Blue" || color == "BLUE") {
                col = "\033[38;2;0;0;255m";
                continue;
            }else if(color == "reset" || color == "Reset" || color == "RESET") {
                col = "\033[0m";
                continue;
            }
            else {
                cout << "Available colors : Red, Green, Blue" << endl;
                continue;
            }
        }

        
        if(input.find("history") != std::string::npos) {
            size_t historyPos = input.find("history");
            if (historyPos + 7 < input.length()) {
                std::string numberString = input.substr(historyPos + 8);
                if (isInt(numberString)) {
                    int number = std::stoi(numberString);
                    string cmd = command_history[number - 1];
                    cout << cmd << endl;
                    if(aliases.count(cmd) > 0) {
                        cmd = aliases[cmd];
                    }
                    execute(cmd, col);
                    continue;
                }
            }
            
            showHistory(command_history, "");
            continue;
        }
        

        command_history.push_back(input);

        if(input == "exit") {
            break;
        }

        if(aliases.count(input) > 0) {
            input = aliases[input];
        }

        if(input.size() >= 3 && input.substr(0, 2) == "cd") {
            string path = input.substr(3);
            cd(path);
            continue;
        }

        stringstream ss(input);
        string command;
        vector<string> arguments;
        getline(ss, command, ' ');
        string argument;

        while(getline(ss, argument,' ')) {
            arguments.push_back(argument);
        }

        string cmd = command;

        for(const string& argument : arguments) {
            cmd += ' ' + argument;
        }

        int num = execute(cmd, col);

    }

    return 0;
}


