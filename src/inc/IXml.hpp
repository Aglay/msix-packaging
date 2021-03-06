//
//  Copyright (C) 2017 Microsoft.  All rights reserved.
//  See LICENSE file in the project root for full license information.
// 
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Exceptions.hpp"
#include "ComHelper.hpp"
#include "StreamBase.hpp"
#include "MSIXFactory.hpp"

// XML file content types/schemas
enum class XmlContentType : std::uint8_t
{
    ContentTypeXml  = 0,
    AppxBlockMapXml = 1,
    AppxManifestXml = 2,
    AppxBundleManifestXml =3,
};

// defines queries for use in IXmlDom->ForEachElementIn
enum class XmlQueryName : std::uint8_t
{
    Package_Identity                           = 0,
    BlockMap_File                              = 1,
    BlockMap_File_Block                        = 2,
    Bundle_Identity                            = 3,
    Bundle_Packages_Package                    = 4,
    Bundle_Packages_Package_Resources_Resource = 5,
    Package_Dependencies_TargetDeviceFamily    = 6,
    Package_Applications_Application           = 7,
    Package_Properties                         = 8,
    Package_Properties_Description             = 9,
    Package_Properties_DisplayName             = 10,
    Package_Properties_PublisherDisplayName    = 11,
    Package_Properties_Logo                    = 12,
    Package_Properties_Framework               = 13,
    Package_Properties_ResourcePackage         = 14,
    Package_Properties_AllowExecution          = 15,
    Package_Dependencies_PackageDependency     = 16,
    Package_Capabilities_Capability            = 17,
    Package_Resources_Resource                 = 18,
};

// defines attribute names for use in IXmlElement-> [GetAttributeValue|GetBase64DecodedAttributeValue]
enum class XmlAttributeName : std::uint8_t
{
    Name                                       = 0,
    ResourceId                                 = 1,
    Version                                    = 2,
    Size                                       = 3,
    Identity_ProcessorArchitecture             = 4,
    Publisher                                  = 5,
    BlockMap_File_LocalFileHeaderSize          = 6,
    BlockMap_File_Block_Hash                   = 7,
    Bundle_Package_FileName                    = 8,
    Bundle_Package_Offset                      = 9,
    Bundle_Package_Type                        = 10,
    Bundle_Package_Architecture                = 11,
    Language                                   = 12,
    MinVersion                                 = 13,
    Dependencies_Tdf_MaxVersionTested          = 14,
    Package_Applications_Application_Id        = 15,
};

// must remain in same order as XmlAttributeName
static const wchar_t* attributeNames[] = {
    /* Name                                   */L"Name",
    /* ResourceId                             */L"ResourceId",
    /* Version                                */L"Version",
    /* Size                                   */L"Size",
    /* Package_Identity_ProcessorArchitecture */L"ProcessorArchitecture",
    /* Publisher                              */L"Publisher",
    /* BlockMap_File_LocalFileHeaderSize      */L"LfhSize",
    /* BlockMap_File_Block_Hash               */L"Hash",
    /* Bundle_Package_FileName                */L"FileName",
    /* Bundle_Package_Offset                  */L"Offset",
    /* Bundle_Package_Type                    */L"Type",
    /* Bundle_Package_Architecture            */L"Architecture",
    /* Language                               */L"Language",
    /* MinVersion                             */L"MinVersion",
    /* Dependencies_Tdf_MaxVersionTested      */L"MaxVersionTested",
    /* Package_Applications_Application_Id    */L"Id",
};

#ifdef USING_MSXML

// must remain in same order as XmlQueryName
static const wchar_t* xPaths[] = {
    /* Package_Identity                           */L"/*[local-name()='Package']/*[local-name()='Identity']",
    /* BlockMap_File                              */L"/*[local-name()='BlockMap']/*[local-name()='File']",
    /* BlockMap_File_Block                        */L"*[local-name()='Block']",
    /* Bundle_Identity                            */L"/*[local-name()='Bundle']/*[local-name()='Identity']",
    /* Bundle_Packages_Package                    */L"/*[local-name()='Bundle']/*[local-name()='Packages']/*[local-name()='Package']",
    /* Bundle_Packages_Package_Resources_Resource */L"*[local-name()='Resources']/*[local-name()='Resource']",
    /* Package_Dependencies_TargetDeviceFamily    */L"/*[local-name()='Package']/*[local-name()='Dependencies']/*[local-name()='TargetDeviceFamily']",
    /* Package_Applications_Application           */L"/*[local-name()='Package']/*[local-name()='Applications']/*[local-name()='Application']",
    /* Package_Properties                         */L"/*[local-name()='Package']/*[local-name()='Properties']",
    /* Package_Properties_Description             */L"*[local-name()='Description']",
    /* Package_Properties_DisplayName             */L"*[local-name()='DisplayName']",
    /* Package_Properties_PublisherDisplayName    */L"*[local-name()='PublisherDisplayName']",
    /* Package_Properties_Logo                    */L"*[local-name()='Logo']",
    /* Package_Properties_Framework               */L"*[local-name()='Framework']",
    /* Package_Properties_ResourcePackage         */L"*[local-name()='ResourcePackage']",
    /* Package_Properties_AllowExecution          */L"*[local-name()='AllowExecution']",
    /* Package_Dependencies_PackageDependency     */L"/*[local-name()='Package']/*[local-name()='Dependencies']/*[local-name()='PackageDependency']",
    /* Package_Capabilities_Capability            */L"/*[local-name()='Package']/*[local-name()='Capabilities']/*[local-name()='Capability']",
    /* Package_Resources_Resource                 */L"/*[local-name()='Package']/*[local-name()='Resources']/*[local-name()='Resource']",
};
#else

// must remain in same order as XmlQueryName
static const char* xPaths[] = {
    /* XmlQueryName::Package_Identity                               */"/Package/Identity",
    /* XmlQueryName::BlockMap_File                                  */"/BlockMap/File",
    /* XmlQueryName::BlockMap_File_Block                            */"./Block",
    /* XmlQueryName::Bundle_Identity                                */"/Bundle/Identity",
    /* XmlQueryName::Bundle_Packages_Package                        */"/Bundle/Packages/Package",
    /* XmlQueryName::Bundle_Packages_Package_Resources_Resource     */"./Resources/Resource",
    /* XmlQueryName::Package_Dependencies_TargetDeviceFamily        */"/Package/Dependencies/TargetDeviceFamily",
    /* XmlQueryName::Package_Applications_Application               */"/Package/Applications/Application",
    /* XmlQueryName::Package_Properties                             */"/Package/Properties",
    /* XmlQueryName::Package_Properties_Description                 */"./Description",
    /* XmlQueryName::Package_Properties_DisplayName                 */"./DisplayName",
    /* XmlQueryName::Package_Properties_PublisherDisplayName        */"./PublisherDisplayName",
    /* XmlQueryName::Package_Properties_Logo                        */"./Logo",
    /* XmlQueryName::Package_Properties_Framework                   */"./Framework",
    /* XmlQueryName::Package_Properties_ResourcePackage             */"./ResourcePackage",
    /* XmlQueryName::Package_Properties_AllowExecution              */"./AllowExecution",
    /* XmlQueryName::Package_Dependencies_PackageDependency         */"/Package/Dependencies/PackageDependency",
    /* XmlQueryName::Package_Capabilities_Capability                */"/Package/Capabilities/Capability",
    /* XmlQueryName::Package_Resources_Resource                     */"/Package/Resources/Resource",
};
#endif

EXTERN_C const IID IID_IXmlElement;
EXTERN_C const IID IID_IXmlDom;
EXTERN_C const IID IID_IXmlFactory;

#ifndef WIN32
// {ac94449e-442d-4bed-8fca-83770c0f7ee9}
interface IXmlElement : public IUnknown
#else
#include "Unknwn.h"
#include "Objidl.h"
class IXmlElement : public IUnknown
#endif
// An internal interface for XML elements
{
public:
    virtual std::string               GetAttributeValue(XmlAttributeName attribute) = 0;
    virtual std::vector<std::uint8_t> GetBase64DecodedAttributeValue(XmlAttributeName attribute) = 0;
    virtual std::string               GetText() = 0;
};

struct XmlVisitor
{        
    typedef bool(*lambda)(void*, const MSIX::ComPtr<IXmlElement>& );

    void*   context;
    lambda  Callback;

    XmlVisitor(void* c, lambda f) : context(c), Callback(f) {}
};

#ifndef WIN32
// {0e7a446e-baf7-44c1-b38a-216bfa18a1a8}
interface IXmlDom : public IUnknown
#else
class IXmlDom : public IUnknown
#endif
// An internal interface for XML document object model
{
public:
    virtual MSIX::ComPtr<IXmlElement> GetDocument() = 0;
    virtual bool ForEachElementIn(
        const MSIX::ComPtr<IXmlElement>& root,
        XmlQueryName query,
        XmlVisitor&  visitor
    ) = 0;
};

#ifndef WIN32
// {f82a60ec-fbfc-4cb9-bc04-1a0fe2b4d5be}
interface IXmlFactory : public IUnknown
#else
class IXmlFactory : public IUnknown
#endif
// An internal interface for creating an IXmlDom object as well as managing XML services lifetime
{
public:
    virtual MSIX::ComPtr<IXmlDom> CreateDomFromStream(XmlContentType footPrintType, const MSIX::ComPtr<IStream>& stream) = 0;
};

SpecializeUuidOfImpl(IXmlElement);
SpecializeUuidOfImpl(IXmlDom);
SpecializeUuidOfImpl(IXmlFactory);

namespace MSIX {
    MSIX::ComPtr<IXmlFactory> CreateXmlFactory(IMsixFactory* factory);

    template <class T>
    static T GetNumber(const ComPtr<IXmlElement>& element, XmlAttributeName attribute, T defaultValue)
    {
        const auto& attributeValue = element->GetAttributeValue(attribute);
        bool hasValue = !attributeValue.empty();
        T value = defaultValue;
        if (hasValue) { value = static_cast<T>(std::stoul(attributeValue)); }
        return value;        
    }
}
