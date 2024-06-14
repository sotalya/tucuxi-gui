/* 
 * Tucuxi - Tucuxi-core library and command line tool. 
 * This code allows to perform prediction of drug concentration in blood 
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


#ifndef EZXMLREADER_H
#define EZXMLREADER_H

#include <QCoreApplication>
#include <QXmlDefaultHandler>
#include <QString>
#include <QMap>
#include <QList>
#include <QMultiMap>
#include <QFile>
#include <QMutex>
#include <QVariant>

namespace Tucuxi {
namespace Gui {
namespace Core {



//! \brief Tucuxi simple XML attribute filter
/** \ingroup utils
   This structure defines a filter on a tag.
   A filter is used to skip a tag when it doesn't fit the filter.
   A filter is based on an attribute value.
  */
struct EzFilter {
   //! Name of the tag to apply (no tag name means active on all tags)
   QString name;
   //! The attribute the filter is applied on
   QString attribute;
   //! The value the attribute must have to be processed
   QString value;
};

//! Tucuxi simple XML tag
/** \ingroup utils
   This structure contains basic data about a tag.
  */
struct EzTag {
   //! Name of the tag
   QString name;
   //! Tag's content
   QString content;
   //! Tag's attributes
   QMap<QString,QString> attributes;
   //! Tag's parent
   EzTag *parent;
   //! Tag's childs <child tag name, child complete tag>
   QMultiMap<QString,EzTag*> childs;

   //The default creator initalise the pointers
   EzTag () : parent(0) { }
};

//! Tucuxi simple XML reader
/** \ingroup utils
   This simple XML reader will read and do basic checks on the XML file.
   With a simple structure declaration, most of the job can be done.
   A simple map of the tags name and their parent is sufficient.
   Additionaly, a list of attribute a tag must have is given.
   Mutex are used to avoid multiple task from setting variables when a reading is done.
   A parsing will prevent each other 'set' function to be run, callers will wait until the reading is done.
  */
class EzXMLReader : public QXmlDefaultHandler
{
    Q_DECLARE_TR_FUNCTIONS(EzXMLReader)
public:

   EzXMLReader();

   //! \brief Set the language
   /** If the language is not set when reading, no filtering is done.
      The language is used as follow : if a tag with a 'lang' attribute is found, the language is checked.
      If no other tag with the same name with the same tag as parent is set, the tag is saved in the resulting tree.
      The language preference order is : the language defined, english, and if nothing else is found, the first tag of this type will be taken.
      @param lang Language to use
     */
   void setLanguage(QString _lang);

   //! \brief Set the XML file
   /** The file will be read even if the extension is wrong.
      @param filename The XML file name and path
      @return True if the file exists and can be opened, false if any error occurs
     */
   bool setFile (QString filename);

   //! \brief Set the list of tag to ignore
   /** The tag and their content (even other tags inside it) will be ignored.
     @param ignored List of tag name to ignore
     */
   void setIgnoredTags (const QList<QString> &ignored);

   //! \brief Set the XML structure
   /** If no strucure is given, no check is done.
      The structure is a map of possible tag names with the name of the tag parent.
      This way, a pseudo-tree structure can be define.
      If a same tag can be found in different places, it need to be defined and associated with each of the possible parents.
      This structure is used only for checking, and if a tag is found multiple times under the same parent tag, no error will be raised.
      @param XMLStruct The map representing the XML structure
     */
   void setStructure (const QMultiMap<QString,QString> &XMLStruct);

   //! \brief Set the necessary tag attributes
   /** Only the necessary attributes for a specific tag needs to be defined.
      All the attributes will be recorded in the resulting tree anyway, the purpose here is to check if a necessary tag is missing.
     */
   void setTagAttributes (const QMultiMap<QString,QString> &tagAtts);

   //! Set a tag stopping the parsing
   /** After this tag has been read, the parsing ends.
     The parsing will stop even if the tag is one of the ignored ones.
     @param tagName The tag to stop at
     */
   void setStopTag (QString tagName);

   //! Set a list of filter
   /** All the tag that does not complies with the filter is ignored (with its sub-content)
     @param filters List of filter to apply
     */
   void setFilters (const QList<EzFilter> &filters);

   //! \brief Read and parse the XML
   /** The resulting tree will be build here.
      This function needs to be called after all the necessary XML description has been given.
      A call to this function is bloquant, and a mutex prevents any other 'set' function to be called before the and of the parsing.
      @return True if the parsing went fine, false elsewhere
     */
   bool readXML ();

   //! \brief Return the resulting XML tree
   /** A link of the root of the XML tree is given.
      Each element contains its childs, and resulting data can be searched through it.
      @return The built XML tree root
     */
   EzTag* result ();

   //! \brief Reset the reader
   /** Erase all the document-related values.
     */
   void reset ();

   //! \brief Check an attribute for its presence and its validity
   /** If the object is not present or not valid, the default value is returned and the string is filled with the error found.
      @param root The tag in which to search the attribute
      @param attribute The attribute to fetch
      @param type The type in which the data is supposed to be
      @param value The default value for the data
      @param errors The error strings is returned in this parameter if anything goes wrong
     */
   QVariant fetchAttribute (EzTag* root, QString attribute, int type, QVariant value = QVariant(), QStringList *error = 0);

   //! \brief Print the XML tree in a string
   /** The tree will be printed as a valid XML in a string.
      @param root The XML tree to print
      @param level XML current level, used only internally, should be ignored
      @return The XML computed file in a string
     */
   static QString print (EzTag* root, int level=0);

   //! \brief Find tags in an XML tree
   /** This static function ease the search of a particular tag among an XML tree.
      @param root Base of the XML tree to search
      @param tagName The name of the tag to find
      @return A list of tags found in the tree
     */
   static QList<EzTag*> find (EzTag* root, QString tagName);

   /** Delete the tree from, and including, the given tag.
    * @param root The base of the tree to delete
    */
   static void deleteTree (EzTag* root);

   //! \brief Delete the whole tree and set the root to 0
   void deleteTree ();

private:
   // The XML file
   QFile *_file;
   // Language to use for reading
   QString _lang;
   // XML file structure <tag,parent tag>
   QMultiMap<QString,QString> _struct;
   // Tag necessary attributes <tag,attribute>
   QMultiMap<QString,QString> _attributes;
   // List of tags to ignore
   QList<QString> _ignored;
   // Tag to stop after
   QString _stopAt;
   //List of filters
   QList<EzFilter> _filters;


   // XML tree, root
   EzTag *_tree;
   // Mutex, used when reading
   QMutex _mutex;
   // The used XML reader
   QXmlSimpleReader *_reader;
   // The tag we're actually in
   EzTag *_crtEzTag;
   // Level to ignore (tag)
   int _crtIgnore;
   // Level of a lang element
   int _crtLangDeep;
   // Deepness of the current tag
   int _deepness;
   // Will be set to true if the parsing has been stoped by us
   bool _stopped;


   //XML handlers stuff
   bool fatalError (const QXmlParseException & exception);
   bool startDocument ();
   bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
   bool characters ( const QString & ch );
   bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );

};

} // namespace Core
} // namespace Gui
} // namespace Tucuxi

#endif // EZXMLREADER_H
