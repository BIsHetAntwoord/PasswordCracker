#include "debug.hpp"
#include "dictionary.hpp"

#include <iostream>

void print_dictionary(std::ostream& os, const Dictionary& dict)
{
    for(const DictionaryElement* element : dict)
    {
        os << "Dictionary element: " << std::endl;
        for(const std::vector<DictionaryElementPart>& elem_parts : *element)
        {
            os << "  Part sequence: " << std::endl;
            for(const DictionaryElementPart& part : elem_parts)
            {
                os << "    ";
                for(size_t i = 0; i < part.alias_count(); ++i)
                    os << part.alias(i) << " ";
                os << std::endl;
            }
        }
        os << std::endl;
    }
}
