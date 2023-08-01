#include "Serializer.h"

#include "tinyxml2.cpp"
#include "tinyxml2.h"

#include "Log/Log.h"

Serializer::Serializer(const std::string& rootElementName) : m_RootElementName(rootElementName) 
{
    m_Document = new tinyxml2::XMLDocument();

    m_Document->NewDeclaration();
    m_RootElement = m_Document->NewElement(rootElementName.c_str());
    m_Document->InsertFirstChild(m_RootElement);
}

void Serializer::AddChildElement(const std::string& elementName, const std::string& text) 
{
    tinyxml2::XMLElement* element = m_Document->NewElement(elementName.c_str());
    if (!text.empty()) 
    {
        element->SetText(text.c_str());
    }
    m_RootElement->InsertEndChild(element);
}

// Add an attribute to the last added child element
void Serializer::AddAttribute(const std::string& attributeName, const std::string& attributeValue)
{
    if (m_CurrentElement) 
    {
        m_CurrentElement->SetAttribute(attributeName.c_str(), attributeValue.c_str());
    }
    else 
    {
        LE_CORE_ERROR("Error: No child element exists to add attribute");
    }
}


// Save the XML document to a file
bool Serializer::SaveToFile(const std::string& fileName) 
{
    tinyxml2::XMLError error = m_Document->SaveFile(fileName.c_str());
    return error == tinyxml2::XML_SUCCESS;
}

void Serializer::SetCurrentElement() 
{
    if (!m_RootElement->NoChildren()) 
    {
        m_CurrentElement = m_RootElement->LastChildElement();
    }
}