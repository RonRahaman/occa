#ifndef OCCA_LANG_VARIABLE_HEADER
#define OCCA_LANG_VARIABLE_HEADER

#include <occa/dtype.hpp>
#include <occa/lang/type.hpp>

namespace occa {
  namespace lang {
    class attribute_t;

    //---[ Variable ]-------------------
    class variable_t {
    public:
      vartype_t vartype;
      identifierToken *source;
      attributeTokenMap attributes;
      std::string nameOverride;

      variable_t();

      variable_t(const vartype_t &vartype_,
                 const std::string &name_);

      variable_t(const vartype_t &vartype_,
                 identifierToken *source_ = NULL);

      variable_t(const variable_t &other);
      variable_t& operator = (const variable_t &other);

      ~variable_t();

      bool isNamed() const;

      std::string& name();
      const std::string& name() const;

      void setName(const std::string &name_);

      variable_t& clone() const;

      bool operator == (const variable_t &other) const;

      bool hasAttribute(const std::string &attr) const;

      // Qualifiers
      bool has(const qualifier_t &qualifier) const;

      variable_t& operator += (const qualifier_t &qualifier);
      variable_t& operator -= (const qualifier_t &qualifier);
      variable_t& operator += (const qualifiers_t &qualifiers);

      void add(const fileOrigin &origin,
               const qualifier_t &qualifier);

      void add(const qualifierWithSource &qualifier);

      void add(const int index,
               const fileOrigin &origin,
               const qualifier_t &qualifier);

      void add(const int index,
               const qualifierWithSource &qualifier);

      // Pointers
      variable_t& operator += (const pointer_t &pointer);
      variable_t& operator += (const pointerVector &pointers);

      variable_t& operator += (const array_t &array);
      variable_t& operator += (const arrayVector &arrays);

      dtype_t dtype() const;

      void printDeclaration(printer &pout) const;
      void printExtraDeclaration(printer &pout) const;

      void printWarning(const std::string &message) const;
      void printError(const std::string &message) const;
    };

    printer& operator << (printer &pout,
                          const variable_t &var);
    //==================================

    //---[ Variable Declaration ]-------
    class variableDeclaration {
      // Note: Feeing of variable and value are delegated
      //         to the declarationStatement
    public:
      variable_t *variable;
      exprNode *value;

      variableDeclaration();

      variableDeclaration(variable_t &variable_,
                          exprNode *value_ = NULL);

      variableDeclaration(variable_t &variable_,
                          exprNode &value_);

      variableDeclaration(const variableDeclaration &other);

      ~variableDeclaration();

      variableDeclaration clone() const;

      void clear();

      bool hasValue() const;

      void print(printer &pout) const;
      void printAsExtra(printer &pout) const;

      void printWarning(const std::string &message) const;
      void printError(const std::string &message) const;
    };
    //==================================
  }
}

#endif
