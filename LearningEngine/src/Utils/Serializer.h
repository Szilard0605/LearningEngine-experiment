#pragma once

#include "tinyxml2.h"
#include <string>
#include <vector>
#include <iostream>

class Serializer
{
public:
    Serializer(const std::string& rootElementName) : rootElementName(rootElementName) {
        xmlDoc.NewDeclaration();
        rootElement = xmlDoc.NewElement(rootElementName.c_str());
        xmlDoc.InsertFirstChild(rootElement);
    }

    // Add a child element to the root
    void AddChildElement(const std::string& elementName, const std::string& text = "") {
        tinyxml2::XMLElement* element = xmlDoc.NewElement(elementName.c_str());
        if (!text.empty()) {
            element->SetText(text.c_str());
        }
        rootElement->InsertEndChild(element);
    }

    // Add an attribute to the last added child element
    void AddAttribute(const std::string& attributeName, const std::string& attributeValue) {
        if (currentElement) {
            currentElement->SetAttribute(attributeName.c_str(), attributeValue.c_str());
        } else {
            std::cerr << "Error: No child element exists to add attribute.\n";
        }
    }

    // Save the XML document to a file
    bool SaveToFile(const std::string& fileName) {
        tinyxml2::XMLError error = xmlDoc.SaveFile(fileName.c_str());
        return error == tinyxml2::XML_SUCCESS;
    }

    // Set the last added child element as the current element
    void SetCurrentElement() {
        if (!rootElement->NoChildren()) {
            currentElement = rootElement->LastChildElement();
        }
    }

private:
    tinyxml2::XMLDocument xmlDoc;
    tinyxml2::XMLElement* rootElement;
    tinyxml2::XMLElement* currentElement = nullptr;
    std::string rootElementName;
};
