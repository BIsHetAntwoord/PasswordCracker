#include "dictionary.hpp"
#include "dictutils.hpp"

//Dictionary element parts

DictionaryElementPart::DictionaryElementPart(const std::string& str)
{
    this->aliases.push_back(str);
}

DictionaryElementPart::DictionaryElementPart(size_t val)
{
    this->aliases.push_back(std::to_string(val));
}
DictionaryElementPart::~DictionaryElementPart() {}

std::string DictionaryElementPart::alias(size_t index) const
{
    return this->aliases[index];
}

size_t DictionaryElementPart::alias_count() const
{
    return this->aliases.size();
}

void DictionaryElementPart::add_alias(const std::string& str)
{
    this->aliases.push_back(str);
}

void DictionaryElementPart::add_alias(size_t val)
{
    this->aliases.push_back(std::to_string(val));
}

bool operator<(const DictionaryElementPart& lhs, const DictionaryElementPart& rhs)
{
    if(lhs.alias_count() == 0 && rhs.alias_count() == 0)
        return false;
    else if(lhs.alias_count() == 0)
        return true;
    else if(lhs.alias_count() == 0)
        return false;
    return lhs.alias(0) < rhs.alias(0);
}

//Dictionary elements
DictionaryElement::~DictionaryElement() {}

std::vector<DictionaryElementPart> DictionaryElement::part_combination(size_t index) const
{
    return this->part_combinations[index];
}

size_t DictionaryElement::part_combination_count() const
{
    return this->part_combinations.size();
}

DictionaryElement::iterator DictionaryElement::begin()
{
    return this->part_combinations.begin();
}

DictionaryElement::iterator DictionaryElement::end()
{
    return this->part_combinations.end();
}

DictionaryElement::const_iterator DictionaryElement::begin() const
{
    return this->part_combinations.begin();
}

DictionaryElement::const_iterator DictionaryElement::end() const
{
    return this->part_combinations.end();
}

DictionaryElement::const_iterator DictionaryElement::cbegin() const
{
    return this->part_combinations.cbegin();
}

DictionaryElement::const_iterator DictionaryElement::cend() const
{
    return this->part_combinations.cend();
}

//Words
Word::Word(const std::string& word)
{
    DictionaryElementPart full_word(word);
    if(!islowercase(word))
        full_word.add_alias(lowercase(word));
    this->part_combinations.push_back({full_word});
}

Word::Word(const std::vector<std::string>& aliases)
{
    DictionaryElementPart full_word(aliases[0]);
    if(!islowercase(aliases[0]))
        full_word.add_alias(lowercase(aliases[0]));

    for(size_t i = 1; i < aliases.size(); ++i)
    {
        full_word.add_alias(aliases[i]);
        if(!islowercase(aliases[i]))
            full_word.add_alias(aliases[i]);
    }

    this->part_combinations.push_back({full_word});
}

Word::~Word() {}

//Names
Name::Name(const std::string& name)
{
    std::vector<std::string> name_parts_str = split_words(name);

    std::vector<DictionaryElementPart> name_parts;
    for(std::string& str : name_parts_str)
    {
        DictionaryElementPart current_name(str);

        std::vector<std::string> word_syllables = split_syllables(str);
        std::string total;
        for(std::string& str : word_syllables)
        {
            total += str;
            current_name.add_alias(total);
            if(!islowercase(total))
                current_name.add_alias(lowercase(total));
        }

        if(word_syllables.size() > 0)
        {
            size_t first_syllable_length = word_syllables[0].size();
            if(first_syllable_length > 2)
            {
                std::string substring = word_syllables[0].substr(0, 2);
                current_name.add_alias(substring);
                if(!islowercase(substring))
                    current_name.add_alias(lowercase(substring));
            }
            if(first_syllable_length > 3)
            {
                std::string substring = word_syllables[0].substr(0, 3);
                current_name.add_alias(substring);
                if(!islowercase(substring))
                    current_name.add_alias(lowercase(substring));
            }
        }

        name_parts.push_back(current_name);
    }

    //Full name
    this->part_combinations.push_back(name_parts);

    if(name_parts.size() > 1)
    {
        //Separate parts
        for(DictionaryElementPart& part : name_parts)
            this->part_combinations.push_back({part});
    }

    //First + Last name
    if(name_parts.size() > 2)
        this->part_combinations.push_back({name_parts[0], name_parts[name_parts.size() - 1]});
}

Name::~Name() {}

//Date
Date::Date(size_t day, size_t month, size_t year)
{
    DictionaryElementPart day_part(day);
    if(day < 10)
        day_part.add_alias("0" + std::to_string(day));

    DictionaryElementPart month_part(month);
    if(month < 10)
        month_part.add_alias("0" + std::to_string(month));

    DictionaryElementPart year_part(year);
    year_part.add_alias(year % 100);
    if(year % 1000 < 100)
        year_part.add_alias(std::to_string(year % 1000) + "k" + std::to_string(year % 100));

    this->part_combinations.push_back({day_part, month_part, year_part});
    this->part_combinations.push_back({day_part});
    this->part_combinations.push_back({month_part});
    this->part_combinations.push_back({year_part});
    this->part_combinations.push_back({day_part, month_part});
    this->part_combinations.push_back({month_part, year_part});
    this->part_combinations.push_back({day_part, year_part});
}

Date::~Date() {}

//Dictionary

Dictionary::Dictionary() {}
Dictionary::Dictionary(Dictionary&& dict) : elements(std::move(dict.elements)) {}
Dictionary::~Dictionary()
{
    for(DictionaryElement* element : this->elements)
    {
        delete element;
    }
}

Dictionary::iterator Dictionary::begin()
{
    return this->elements.begin();
}

Dictionary::iterator Dictionary::end()
{
    return this->elements.end();
}

Dictionary::const_iterator Dictionary::begin() const
{
    return this->elements.begin();
}

Dictionary::const_iterator Dictionary::end() const
{
    return this->elements.end();
}

Dictionary::const_iterator Dictionary::cbegin() const
{
    return this->elements.cbegin();
}

Dictionary::const_iterator Dictionary::cend() const
{
    return this->elements.cend();
}

void Dictionary::add_word(const std::string& word)
{
    this->elements.push_back(new Word(word));
}

void Dictionary::add_word(const std::vector<std::string>& word_aliases)
{
    this->elements.push_back(new Word(word_aliases));
}

void Dictionary::add_name(const std::string& name)
{
    this->elements.push_back(new Name(name));
}

void Dictionary::add_date(size_t day, size_t month, size_t year)
{
    this->elements.push_back(new Date(day, month, year));
}

size_t Dictionary::size() const
{
    return this->elements.size();
}

DictionaryElement* Dictionary::get_element(size_t i)
{
    return this->elements[i];
}

const DictionaryElement* Dictionary::get_element(size_t i) const
{
    return this->elements[i];
}
