#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

class Serializer
{
public:
    Serializer(const std::string& rootElementName);

    // Add a child element to the root
    void AddChildElement(const std::string& elementName, const std::string& text = "");

    // Add an attribute to the last added child element
    void AddAttribute(const std::string& attributeName, const std::string& attributeValue);

    // Save the XML document to a file
    bool SaveToFile(const std::string& fileName);

    // Set the last added child element as the current element
    void SetCurrentElement();

private:
    tinyxml2::XMLDocument* m_Document;
    tinyxml2::XMLElement* m_RootElement;
    tinyxml2::XMLElement* m_CurrentElement = nullptr;
    std::string m_RootElementName;
};
