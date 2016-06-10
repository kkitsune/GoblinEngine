/****************************************************************************
**
** This file is part of the Ponder library, formerly CAMP.
**
** The MIT License (MIT)
**
** Copyright (C) 2009-2014 TEGESO/TEGESOFT and/or its subsidiary(-ies) and mother company.
** Copyright (C) 2015-2016 Billy Quith.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
**
****************************************************************************/


#include <ponder/detail/classmanager.hpp>
#include <ponder/class.hpp>


namespace ponder
{
namespace detail
{
    
ClassManager& ClassManager::instance()
{
    static ClassManager cm;
    return cm;
}

Class& ClassManager::addClass(const std::string& id)
{
    // First make sure that the class doesn't already exist
    if (classExists(id))
    {
        PONDER_ERROR(ClassAlreadyCreated(id));
    }

    // Create the new class
    Class *newClass = new Class(id);

    // Insert it into the table
    m_classes.insert(std::make_pair(id, newClass));

    // Notify observers
    notifyClassAdded(*newClass);

    // Done
    return *newClass;
}

void ClassManager::removeClass(const Class& cls)
{
    if (!classExists(cls.name()))
    {
        PONDER_ERROR(ClassNotFound(cls.name()));
    }

    ClassTable::const_iterator it = m_classes.find(cls.name());
    Class* classPtr = it->second;

    // Notify observers
    notifyClassRemoved(*classPtr);
        
    delete classPtr;
    m_classes.erase(it);
}
    
std::size_t ClassManager::count() const
{
    return m_classes.size();
}

const Class& ClassManager::getByIndex(std::size_t index) const
{
    // Make sure that the index is not out of range
    if (index >= m_classes.size())
        PONDER_ERROR(OutOfRange(index, m_classes.size()));

    ClassTable::const_iterator it = m_classes.begin();
    std::advance(it, index);

    return *it->second;
}

const Class& ClassManager::getById(const std::string& id) const
{
    ClassTable::const_iterator it = m_classes.find(id);
    if (it == m_classes.end())
        PONDER_ERROR(ClassNotFound(id));

    return *it->second;
}

const Class* ClassManager::getByIdSafe(const std::string& id) const
{
    ClassTable::const_iterator it = m_classes.find(id);
    return (it == m_classes.end()) ? nullptr : it->second;
}

bool ClassManager::classExists(const std::string& id) const
{
    return m_classes.find(id) != m_classes.end();
}

ClassManager::ClassManager()
{
}

ClassManager::~ClassManager()
{
    // Notify observers
    for (ClassTable::const_iterator it = m_classes.begin(); it != m_classes.end(); ++it)
    {
        Class* classPtr = it->second;
        notifyClassRemoved(*classPtr);
        delete classPtr;
    }
}

} // namespace detail

} // namespace ponder
