#ifndef DICTIONARY_HPP_INCLUDED
#define DICTIONARY_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <string>

class DictionaryElementPart
{
    private:
        std::vector<std::string> aliases;
    public:
        DictionaryElementPart(const std::string&);
        DictionaryElementPart(size_t);
        ~DictionaryElementPart();

        std::string alias(size_t) const;
        size_t alias_count() const;

        void add_alias(const std::string&);
        void add_alias(size_t);
};

bool operator<(const DictionaryElementPart&, const DictionaryElementPart&);

class DictionaryElement
{
    protected:
        std::vector<std::vector<DictionaryElementPart>> part_combinations;
    public:
        typedef std::vector<std::vector<DictionaryElementPart>>::iterator iterator;
        typedef std::vector<std::vector<DictionaryElementPart>>::const_iterator const_iterator;

        virtual ~DictionaryElement() = 0;

        std::vector<DictionaryElementPart> part_combination(size_t) const;
        size_t part_combination_count() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
};

class Word : public DictionaryElement
{
    public:
        Word(const std::string&);
        Word(const std::vector<std::string>&);
        ~Word();
};

class Name : public DictionaryElement
{
    public:
        Name(const std::string&);
        ~Name();
};

class Date : public DictionaryElement
{
    public:
        Date(size_t, size_t, size_t);
        ~Date();
};


class Dictionary
{
    private:
        std::vector<DictionaryElement*> elements;
    public:
        typedef std::vector<DictionaryElement*>::iterator iterator;
        typedef std::vector<DictionaryElement*>::const_iterator const_iterator;

        Dictionary();
        Dictionary(Dictionary&&);
        Dictionary(const Dictionary&) = delete;
        ~Dictionary();

        Dictionary& operator=(const Dictionary&) = delete;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

        size_t size() const;
        DictionaryElement* get_element(size_t);
        const DictionaryElement* get_element(size_t) const;

        void add_word(const std::string&);
        void add_word(const std::vector<std::string>&);
        void add_name(const std::string&);
        void add_date(size_t, size_t, size_t);
};

#endif // DICTIONARY_HPP_INCLUDED
