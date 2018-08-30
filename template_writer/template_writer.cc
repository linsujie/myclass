#include "template_writer.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

using namespace std;

bool template_writer::load_template(const std::string& template_file_name_)
{
  template_file_name = template_file_name_;
  return load_template();
}
bool template_writer::load_template()
{
  ifstream template_file(template_file_name);
  string template_str((istreambuf_iterator<char>(template_file)), istreambuf_iterator<char>());

  std::smatch m;
  while (regex_search(template_str, m, regex("%[A-Za-z0-9_]+"))) {
    if (!m.prefix().str().empty()) {
      template_file_text.emplace_back(m.prefix());
      template_status.emplace_back("text");
    }

    string key = m[0];
    template_file_text.emplace_back(key.substr(1, key.size() - 1));
    template_status.emplace_back("variable");

    template_str = m.suffix().str();
  }

  if (!template_str.empty()) {
    template_file_text.emplace_back(template_str);
    template_status.emplace_back("text");
  }

  return true;
}

bool template_writer::show() const
{
  for (unsigned i = 0; i < template_file_text.size(); i++)
    cout << template_status[i] << ": {" << template_file_text[i] << "}" << endl;
  return true;
}

template<typename T> string template_writer::format(const map<string, T>& values) const
{
  ostringstream result;
  for (unsigned i = 0; i < template_status.size(); i++)
    if (template_status[i] == "variable") {
      auto iter = values.find(template_file_text[i]);
      if (iter != values.end()) result << (iter->second);
      else {
        cerr << "template_writer::Warning::variable \"" << template_file_text[i] << "\" required in file " << template_file_name << " was not given." << endl;
        result << "%" << template_file_text[i];
      }

    } else
      result << template_file_text[i];

  return result.str();
}


    template string template_writer::format<string>(const map<string, string>& values) const;
    template string template_writer::format<double>(const map<string, double>& values) const;
