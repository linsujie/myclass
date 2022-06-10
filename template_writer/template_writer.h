#ifndef TEMPLATE_WRITER_H
#define TEMPLATE_WRITER_H

#include <string>
#include <vector>
#include <map>

class template_writer {
  private:
    std::string template_file_name;
    std::vector<std::string> template_file_text, template_status;
    bool load_template();

  public:
    template_writer(const std::string& template_file_name_) : template_file_name(template_file_name_) { load_template(); }
    bool load_template(const std::string& template_file_name_);

    bool show() const;
    template<typename T> std::string format(const std::map<std::string, T>& values) const;
};

#endif /* TEMPLATE_WRITER_H */
