/* 
 * Tucuxi - Tucuxi-gui software. 
 * This software is able to perform prediction of drug concentration in blood 
 * and to propose dosage adaptations.
 * It has been developed by HEIG-VD, in close collaboration with CHUV. 
 * Copyright (C) 2024 HEIG-VD, maintained by Yann Thoma  <yann.thoma@heig-vd.ch>
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Affero General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU Affero General Public License for more details. 
 * 
 * You should have received a copy of the GNU Affero General Public License 
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "core/utils/ezxmlreader.h"

//#include "core_errors.h"
//#include "coremessagehandler.h"
#include "core/core.h"
#include "convert.h"
#include "core/utils/logging.h"
#include "core/errors_core.h"

#include <QObject>
#include <QXmlInputSource>
#include <QXmlAttributes>
#include <QFile>
#include <QMetaType>

namespace Tucuxi {
namespace Gui {
namespace Core {



//Create the reader
EzXMLReader::EzXMLReader() : _reader(new QXmlSimpleReader()), _file(0), _tree(0), _crtEzTag(0)
{
   //Set the XML reader properties
   _reader->setContentHandler(this);
   _reader->setErrorHandler(this);
   _reader->setFeature("http://trolltech.com/xml/features/report-whitespace-only-CharData",false);
}

void EzXMLReader::setLanguage(QString lang)
{
   //Block the access when parsing
   _mutex.lock();
   //Set
   this->_lang = lang;
   //Unlock
   _mutex.unlock();
}

/* Try to open the file, and set if ok */
bool EzXMLReader::setFile(QString filename)
{
   //Try to open the file
   QFile *newFile = new QFile(filename);
   if (!newFile->exists()) {
      LOG(QtWarningMsg, NOEZERROR, tr("The XML file does not exists : ")+filename);
      return false;
   }

   //Block the access when parsing
   _mutex.lock();
   _file = newFile;
   //Unlock
   _mutex.unlock();

   return true;
}

void EzXMLReader::setIgnoredTags(const QList<QString> &ignored)
{
   //Block the access when parsing
   _mutex.lock();
   //Set
   this->_ignored = ignored;
   //Unlock
   _mutex.unlock();
}

void EzXMLReader::setStructure(const QMultiMap<QString, QString> &XMLStruct)
{
   //Block the access when parsing
   _mutex.lock();
   //Set
   this->_struct = XMLStruct;
   //Unlock
   _mutex.unlock();
}

void EzXMLReader::setTagAttributes(const QMultiMap<QString, QString> &tagAtts)
{
   //Block the access when parsing
   _mutex.lock();
   //Set
   this->_attributes = tagAtts;
   //Unlock
   _mutex.unlock();
}

void EzXMLReader::setStopTag(QString tagName)
{
   //Block the access when parsing
   _mutex.lock();
   //Set
   _stopAt = tagName;
   //Unlock
   _mutex.unlock();
}

void EzXMLReader::setFilters(const QList<EzFilter> &filters)
{
   //Block the access when parsing
   _mutex.lock();
   //Set
   _filters = filters;
   //Unlock
   _mutex.unlock();
}

//Start the XML parsing
bool EzXMLReader::readXML()
{
   //Check file
   if (!_file) {
      LOG(QtWarningMsg, NOEZERROR, tr("The XML file has not been set"));
      return false;
   }

   //Lock the object for the parsing to avoid multiple thread to built invalid cross data
   _mutex.lock();

   //Open the file
   if (!_file->open(QFile::ReadOnly)) {
      LOG(QtWarningMsg, NOEZERROR, tr("The XML file cannot be opened")+" : "+_file->fileName());
      return false;
   }
   //Bind the file to the source
   QXmlInputSource *source = new QXmlInputSource(_file);
   //Start the parsing, if an error has occured, return null (the message error has already been sent)
   //If the stop has been caused by _stopAt, ignore
   bool ok = _reader->parse(source) || _stopped;

   //Destroy the source and close the file
   delete source;
   _file->close();
   _mutex.unlock();
   return ok;
}

//Return the last result tree
EzTag* EzXMLReader::result()
{
   return _tree;
}

//Clear all the document-related values
void EzXMLReader::reset()
{
   //Block the access when parsing
   _mutex.lock();
   //Reset
   delete _file;
   _lang = QString();
   _struct.clear();
   _attributes.clear();
   _ignored.clear();
   _filters.clear();
   _stopAt.clear();
   //Unlock
   _mutex.unlock();
}

//Do the check, return the value
QVariant EzXMLReader::fetchAttribute(EzTag *root, QString attribute, int type, QVariant value, QStringList *errors)
{
   //Check for the presence
   if ( ! root->attributes.contains(attribute) ) {
      if (errors)
         errors->append(tr("The attribute %1 for the tag %2 is missing").arg(attribute, root->name));
      return value;
   }

   //Convert the data
   QVariant data = Convert::fromString(type,root->attributes.value(attribute));

   //Check the type
   if ( ! data.isValid() ) {
      if (errors)
         errors->append(tr("The attribute %1 for the tag %2 is not in the good format (expecting a %3, found : %4)").arg(attribute, root->name, QMetaType::typeName(type), root->attributes.value(attribute)));
      return value;
   }

   //All green, return the value
   return data;
}

//Recursive search for the tag
QList<EzTag*> EzXMLReader::find(EzTag* root, QString tagName)
{
   EzTag *tag;
   QList<EzTag*> list;
   //For all the root childs
   foreach(tag,root->childs) {
      //If this tag corresponds, add it
      if (tag->name == tagName)
         list << tag;
      //Launch recursivity
      list += find(tag,tagName);
   }
   return list;
}

//Delete all leaf, using recursivity
void EzXMLReader::deleteTree(EzTag *root)
{
    //Yeah, we expect something
    if (!root)
        return;

    //If the tag has childrens, launch the recursivity on each children
    foreach(EzTag *tag, root->childs)
        deleteTree(tag);

    //Remove the tag from our parent and destroy it!
    if (root->parent)
        root->parent->childs.remove(root->name, root);

    //Delete the current tag
    delete root;
}

//Delete all the tree
void EzXMLReader::deleteTree()
{
    deleteTree(_tree);
    _tree = 0;
}

/* Recursively print all the element in a QString with \n
  */
QString EzXMLReader::print(EzTag *root, int level)
{
   EzTag *tag;
   QString xml, ind;

   //Indentation
   for (int i=0; i<level; i++)
      ind += "\t";

   //Add the tag, name + attributes + content + end
   xml += ind+"<"+root->name;
   QMap<QString, QString>::const_iterator i = root->attributes.constBegin();
   while (i != root->attributes.constEnd()) {
       xml += " "+i.key()+"=\""+i.value()+"\"";
       ++i;
   }
   xml += ">"+root->content;

   //For all the root childs, launch recursivity
   if (!root->childs.isEmpty())
      xml += "\n";
   foreach(tag,root->childs)
      xml += print(tag,level+1);
   if (!root->childs.isEmpty())
      xml += ind;

   //End
   xml += "</"+root->name+">\n";
   return xml;
}


//----------------------------------- XML FUNCTIONS -------------------------------

//Empty the result tree and all the temporary variables
bool EzXMLReader::startDocument()
{
   _crtIgnore = -1;
   _crtLangDeep = -1;
   _deepness = 0;
   _stopped = false;
   //Root tag
   _crtEzTag = new EzTag;
   _crtEzTag->name = "root";
   _tree = _crtEzTag;
   return true;
}

// If not in head, check the structure, then save the current tag and its attributes
bool EzXMLReader::startElement(const QString&, const QString&, const QString &qName, const QXmlAttributes &atts)
{
   //Increment the deepness
   _deepness++;

   //Skip if the ignore tag is set
   if (_crtIgnore != -1)
      return true;

   //If the current tag is one of the one to ignore, skip
   if (_ignored.contains(qName)) {
      _crtIgnore = _deepness;
      return true;
   }

   //If the current tag doesn't fit the filters, skip
   if (!_filters.isEmpty()) {
      foreach(EzFilter f, _filters)
         if (f.name == qName)
            if (atts.value(f.attribute) != f.value) {
               _crtIgnore = _deepness;
               return true;
            }
   }

   //Balise order check, if the parent (current tag) is not equal to what it should be
   if (!_struct.values(qName).contains(_crtEzTag->name)) {
      QString possible,tmp;
      foreach(tmp,_struct.values(qName))
         possible += '"'+tmp+"\" ";
      //In case no possible tag is found, display a different message
      if (possible.isEmpty()){
         LOG(QtWarningMsg, NOEZERROR, tr("Invalid XML tag found : %1").arg(qName));
      }
      else {
          LOG(QtWarningMsg, NOEZERROR, tr("Invalid XML, the tag \"%1\" should be in a tag %2 but found in a tag \"%3\"").arg(qName,possible,_crtEzTag->name));
      }
      return false;
   }

   //Balise attributes check, a least the defined attributes must be present
   QString att;
   foreach(att,_attributes.values(qName))
      if (atts.index(att) == -1) {
         LOG(QtWarningMsg, NOEZERROR, tr("Missing the attribute %1 in a tag %2 on the file %3").arg(att,qName,_file->fileName()));
         return false;
      }

   //If the language is set, and so activated
   if (!_lang.isEmpty()) {
      //If a 'lang' attribute exists
      int li = atts.index("lang");

      //By default a node is added to the tree, except for the conditions
      //where the currently parsed node is a better fit than the one we
      //already have registered.

      //The main idea is to take the first one found, unless we find one
      //in english (default choice) or better, the one in the language
      //defined in the "lang" member. If we already have a node registered
      //and the current has the same language setting, we keep the one
      //already registered and ignore the currently parsed node.
      if (li != -1) {
        EzTag *registeredChild = _crtEzTag->childs.value(qName, NULL);

        if (registeredChild != NULL) {
          QString currentLang = atts.value(li);
          QString registeredLang = registeredChild->attributes.value("lang");

          // Test ordering is important!
          if (((registeredLang != _lang) && (registeredLang != "en")) &&
              ((currentLang != _lang) && (currentLang != "en")))
          {
            _crtIgnore = _deepness;
            return true;
          }

          if ((currentLang == _lang) && (registeredLang == _lang))
          {
            _crtIgnore = _deepness;
            return true;
          }

          if (((registeredLang == _lang) || (registeredLang == "en"))
                     && (currentLang != _lang))
          {
            _crtIgnore = _deepness;
            return true;
          }

          //Remove the currently registered node, as we will replace
          //it with the current one.
          _crtEzTag->childs.remove(qName);
        }
        //Else register as we have no currently registered node
      }
   }

   //Save the current EzTag
   EzTag *parent = _crtEzTag;

   //Create new EzTag
   _crtEzTag = new EzTag;
   for (int i=0; i<atts.count(); i++)
      _crtEzTag->attributes[atts.qName(i)] = atts.value(i);
   _crtEzTag->name = qName;
   _crtEzTag->parent = parent;

   //Add the new tag as a child of its parent
   parent->childs.insert(qName,_crtEzTag);

   return true;
}

// For each tag, save the correspondant value
bool EzXMLReader::characters(const QString &ch)
{
   //Skip if the tag is not to be ignored, save the content
   if (_crtIgnore == -1)
      _crtEzTag->content = ch.simplified();
   return true;
}

/* Element finished, current tag passes at the parent tag
   This works only if the parent/child relation is unique and checked before
  */
bool EzXMLReader::endElement(const QString&, const QString&, const QString &qName)
{
   //If the tag ending is the stop tag, quit
   if (qName == _stopAt) {
      _stopped = true;
      return false;
   }

   //If the ignore flag is not active, change the current tag
   if (_crtIgnore == -1)
      _crtEzTag = _crtEzTag->parent;
   //If the current level corresponds to the one to ignore, reset crtIgnore
   if (_crtIgnore == _deepness)
      _crtIgnore = -1;
   //Decrement the level deepness
   _deepness--;
   return true;
}

//Not used normally, using our own error system
bool EzXMLReader::fatalError (const QXmlParseException & exception)
{
   //Ignore the wanted stop
   if (!_stopped)
       LOG(QtWarningMsg, NOEZERROR, tr("Fatal error on line %1, column %2 : %3").arg(QString::number(exception.lineNumber()),
                                                                QString::number(exception.columnNumber()),
                                                                exception.message()));
   //We do not continue
   return false;
}


} // namespace Core
} // namespace Gui
} // namespace Tucuxi

