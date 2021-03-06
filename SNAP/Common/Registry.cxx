/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    Registry.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "Registry.h"
#include "IRISVectorTypes.h"

#include <stdio.h>
#include <cstdlib>
#include <cstdarg>
#include <fstream>
#include <iomanip>

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


RegistryValue
::RegistryValue()
{
  m_Null = true;
  m_String = "";  
}

RegistryValue
::RegistryValue(const std::string &input)
{
  m_Null = false;
  m_String = input;  
}

RegistryValue&
Registry::Entry(const std::string &key)
{
  // Get the containing folder
  StringType::size_type iDot = key.find_first_of('.');

  // There is a subfolder
  if(iDot != key.npos)
    {
    StringType child = key.substr(0,iDot);
    StringType childKey = key.substr(iDot+1);
    return Folder(child).Entry(childKey);
    }

  // Search for the key and return it if found
  EntryIterator it = m_EntryMap.find(key);
  if(it != m_EntryMap.end())
    return it->second;

  // Key was not found, create a null entry
  m_EntryMap[key] = RegistryValue();
  return m_EntryMap[key];
}

Registry::StringType
Registry::Key(const char *format,...)
{
  // A string for prinf-ing
  static char buffer[1024];
  
  // Do the printf operation
  va_list al;
  va_start(al,format);
  vsprintf(buffer,format,al);
  va_end(al);

  // Use the string as parameter
  return StringType(buffer);  
}

int
Registry
::GetEntryKeys(StringListType &targetArray) 
{
  // Iterate through keys in ascending order
  for(EntryIterator it=m_EntryMap.begin();it!=m_EntryMap.end();++it)
    {
    // Put the key in the array
    targetArray.push_back(it->first);
    }

  // Return the number of keys copied
  return targetArray.size();
}

int
Registry
::GetFolderKeys(StringListType &targetArray) 
{
  // Iterate through keys in ascending order
  for(FolderIterator it=m_FolderMap.begin();it!=m_FolderMap.end();++it)
    {
    // Put the key in the array
    targetArray.push_back(it->first);
    }

  // Return the number of keys copied
  return targetArray.size();
}

void 
Registry
::Write(std::ostream &sout,const StringType &prefix)
{
  // Write the entries in this folder
  for(EntryIterator ite = m_EntryMap.begin();ite != m_EntryMap.end(); ++ite)
    {
    // Only write the non-null entries
    if(!ite->second.IsNull())
      {
      // Write the key = 
      sout << prefix << Encode(ite->first) << " = ";

      // Write the encoded value
      sout << Encode(ite->second.GetInternalString()) << std::endl;
      }
    }

  // Write the folders
  for(FolderIterator itf = m_FolderMap.begin(); itf != m_FolderMap.end(); ++itf)
    {
    // Write the folder contents (recursive, contents prefixed with full path name)
    itf->second->Write(sout, prefix + itf->first + "." );
    }  
}

void 
Registry
::SetFlagAddIfNotFound(bool yesno) 
{
  // Set the internal value
    m_AddIfNotFound = yesno;

  // Propagate to all the children folders
  for(FolderIterator itf = m_FolderMap.begin(); itf != m_FolderMap.end(); ++itf)
    {
    itf->second->SetFlagAddIfNotFound(yesno);
    }
}

void 
Registry
::CollectKeys(StringListType &keyList,const StringType &prefix) 
{
  // Go through the children
  for(FolderIterator itf = m_FolderMap.begin(); itf != m_FolderMap.end(); ++itf)
    {
    // Collect the child's keys with a new prefix
    itf->second->CollectKeys(keyList, prefix + itf->first + ".");
    }
  
  // Add the keys in this folder
  for(EntryIterator ite = m_EntryMap.begin();ite != m_EntryMap.end(); ++ite)
    {
    // Add the key to the collection list
    keyList.push_back(prefix + ite->first);
    }
}

void 
Registry
::Update(const Registry &reg) 
{
  // Go through the children
  for(FolderIterator itf = reg.m_FolderMap.begin(); 
    itf != reg.m_FolderMap.end(); ++itf)
    {
    // Update the sub-folder
    this->Folder(itf->first).Update(*(itf->second));
    }
  
  // Add the keys in this folder
  for(EntryConstIterator ite = reg.m_EntryMap.begin();
    ite != reg.m_EntryMap.end(); ++ite)
    {
    RegistryValue &entry = Entry(ite->first);
    entry = ite->second;   
    }
}




Registry::StringType 
Registry
::FindValue(const StringType& value)
{
  // Add the keys in this folder
  for(EntryIterator ite = m_EntryMap.begin();ite != m_EntryMap.end(); ++ite)
    {
    if(ite->second.GetInternalString() == value)
      return ite->first;
    }
  return "";
}

void 
Registry
::RemoveKeys(const char *match)
{
  // Create a match substring
  std::string sMatch = (match) ? match : 0;

  // Search and delete from the map
  EntryMapType newMap;
  for(EntryIterator it=m_EntryMap.begin(); it != m_EntryMap.end(); it++)
    {
    if(it->first.find(sMatch) != 0)
      {
      newMap[it->first] = it->second;
        }
    }

  m_EntryMap = newMap;
}

void
Registry
::Clear()
{
  m_EntryMap.clear();
  m_FolderMap.clear();
}


Registry::StringType
Registry
::Encode(const StringType &input) 
{
  IRISOStringStream oss;
  for(unsigned int i=0; i < input.length() ; i++)
    {
    if(!isprint(input[i]) || input[i]=='%' || isspace(input[i]))
      {
      // Replace character by a escape string
      oss << '%';
      oss << std::setw(2) << std::setfill('0') << std::hex << (int)input[i];
      }
    else
      {
      // Just copy the character
      oss << input[i];
      }
    }

  // Return the resulting string
  return oss.str();
}   

Registry::StringType
Registry::Decode(const StringType &input) 
{
  // Create an input stream
  IRISIStringStream iss(input);
  IRISOStringStream oss;

  // Read until we run out or crash
  while(iss.good())
    {  
    // Read the next character
    char c = (char) iss.get();

    // Check if the character needs to be translated
    if(!isprint(c))
      {
      continue;
      }
    else if(c != '%')
      {
      // Just copy the character
      oss.put(c);
      }
    else 
      {
      // A pair of chars
      char c1,c2;
      
      // Read the hex digit
      iss >> c1;
      iss >> c2;
      
      // Reconstruct the hex
      int d1 = (c1 < 'a') ? c1 - '0' : c1 - 'a' + 10;
      int d2 = (c2 < 'a') ? c2 - '0' : c2 - 'a' + 10;
      int digit = d1 * 16 + d2;

      // See if the read succeeded, only then use the digit
      if(iss.good())
        oss << (char)digit;      
      
      // A good place to throw an exception (for strict interpretation)
      }        
    }
  
  // Return the result
  return oss.str();
}


void
Registry
::Read(std::istream &sin, std::ostream &oss) 
{
  unsigned int lineNumber = 1;
  while(sin.good())
    {
    // Read a line from the file
    char buffer[1024];
    sin.getline(buffer,1024);

    // Find the first character in the string
    StringType line = buffer;
    StringType::size_type iToken = line.find_first_not_of(" \t\v\r\n");

    // Skip blank lines
    if(iToken == line.npos)
      continue;

    // Check if it's a comment
    if(line[iToken] == '#')
      continue;

    // Find an equal sign in the string
    StringType::size_type iOper = line.find_first_of('=');
    if(iOper == line.npos)
      {
      // Not a valid line
      oss << std::setw(5) << lineNumber << " : Missing '='; line ignored." << std::endl;
      continue;
      }
    if(iOper == iToken)
      {
      // Missing key
      oss << std::setw(5) << lineNumber << " : Missing key before '='; line ignored." << std::endl;
      continue;
      }

    // Extract the key
    std::string key = Decode(
      line.substr(iToken,line.find_first_of(" \t\v\r\n=",iToken) - iToken));

    // Extract the value
    StringType::size_type iValue = line.find_first_not_of(" \t\v\r\n=",iOper);
    std::string value = "";
    if (iValue != line.npos) 
      {
      value = line.substr(iValue);
      }
 
    // Now the key-value pair is present.  Add it using the normal interface
    Entry(key) = RegistryValue(Decode(value));

    ++lineNumber;
    }
}

Registry &
Registry
::Folder(const std::string &key) 
{
  // Find the first separator in the key string
  StringType::size_type iDot = key.find_first_of('.');

  // Recurse on the immideate folder
  if(iDot != key.npos) 
    {
    StringType child = key.substr(0,iDot);
    StringType childKey = key.substr(iDot+1);
    return Folder(child).Folder(childKey);
    }

  // Get the folder, adding if necessary
  FolderIterator it = m_FolderMap.find(key);
  if(it != m_FolderMap.end())
    return *(it->second);

  // Add the folder
  m_FolderMap[key] = new Registry();
  m_FolderMap[key]->m_AddIfNotFound = m_AddIfNotFound;
  return *m_FolderMap[key];
}

Registry
::Registry()
{
  m_AddIfNotFound = false;
}

Registry
::Registry(const char *fname) 
{
  ReadFromFile(fname);
}

Registry::
~Registry() 
{
  // Delete all the sub-folders
  // for(FolderIterator itf = m_FolderMap.begin(); itf != m_FolderMap.end(); ++itf)
  //  delete itf->second;
}

/**
* Write the folder to a disk
*/
void 
Registry
::WriteToFile(const char *pathname, const char *header) 
{
  // Open the file
  std::ofstream sout(pathname,std::ios::out);

  // Set the stream to be picky
  sout.exceptions(std::ios::failbit);

  // Write the header
  if(header)
    {
    sout << header << std::endl;
    }

  // Write to the stream
  Write(sout,"");
}

/**
* Read folder from file
*/
void 
Registry
::ReadFromFile(const char *pathname) 
{
  // Create an error stream
  IRISOStringStream serr;
      
  // Create output stream
  std::ifstream sin(pathname,std::ios::in);
  if(!sin.good())
    throw IOException("Unable to open the Registry file");

  try 
    {
    // Try reading
    Read(sin,serr);
    }
  catch(...)
    {
    throw IOException("Unable to read the Registry file");
    }

  // If the error stream is not empty, throw an exception
  if(serr.str().length())
    throw SyntaxException(serr.str().c_str());
}



