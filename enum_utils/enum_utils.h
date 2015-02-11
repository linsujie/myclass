#ifndef _ENUM_UTILS_H
#define _ENUM_UTILS_H

#define GETENUM(cls) \
unsigned cls::getenum(const string &enum_name) { \
    for(unsigned i = 0; i < enum_names.size(); i++) \
      for(unsigned j = 0; j < enum_names[i].size(); j++) \
        if(enum_name == enum_names[i][j]) return j; \
  return 1000000;\
}


#endif // for #ifndef _ENUM_UTILS_H
