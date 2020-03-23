<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
    <xsl:output method="html"
        media-type="text/html; charset=UTF-8" 
        doctype-public="-//W3C//DTD XHTML Basic 1.1//EN"
        doctype-system="http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd"
        />
    <xsl:variable name="lang">en</xsl:variable>

    <xsl:template match="/">
        <html>
            <head>
                <title>Drug description: <xsl:value-of select="//head/drugnames/name[@lang=$lang]" /> (<xsl:value-of select="//head/drugmodelid" />)</title>
                <link href="drug.css" type="text/css" rel="stylesheet" />
            </head>

            <body>
                <div id="toc">
                    <p><a href="http://www.ezechiel.ch">http://www.ezechiel.ch</a></p>
                    <br />
                    <p><strong>Table of Content</strong></p>
                    <ol>
                        <a href="#drug"><li><h3><strong>Drug</strong></h3></li></a>
                        <a href="#history"><li>History</li></a>
                        <a href="#adme"><li>ADME</li></a>
                        <a href="#dosages"><li>Dosages</li></a>
                        <a href="#conversions"><li>Conversions</li></a>
                        <a href="#targets"><li>Targets</li></a>
                        <a href="#parameters"><li>Parameters</li></a>
                        <a href="#errormodel"><li>Error model</li></a>
                        <a href="#covariates"><li>Covariates</li></a>
                        <a href="#operations"><li>Operations</li></a>            
                    </ol>
                </div>
                <div id="content">
                    <xsl:apply-templates select="//head" />
                    <xsl:apply-templates select="//history" />
                    <xsl:apply-templates select="//drug" />
                </div>
            </body>
        </html>
    </xsl:template>




    <!-- HEADER STYLING -->
    <xsl:template match="//drug/head">
        <a name="introduction" />
        <a name="drug" />
        <div class="head">
            <a name="{head/id}" />
            <h1><xsl:value-of select="drugnames/name[@lang=$lang]" /></h1>
            <h5><xsl:value-of select="drugmodelid" /></h5>
            <h5>Model: <xsl:value-of select="modelid" />:
            
            <xsl:choose>
                <xsl:when test="modelid = 'ezechiel.model.linear.1comp.bolus'">
                     <a href="http://www.ezechiel.ch/?page_id=185#ezechiel.model.linear.1comp.bolus">Documentation</a>
                </xsl:when>
                <xsl:when test="modelid = 'ezechiel.model.linear.1comp.infu'">
                     <a href="http://www.ezechiel.ch/?page_id=185#ezechiel.model.linear.1comp.infu">Documentation</a>
                </xsl:when>
                <xsl:when test="modelid = 'ezechiel.model.linear.1comp.extra'">
                     <a href="http://www.ezechiel.ch/?page_id=185#ezechiel.model.linear.1comp.extra">Documentation</a>
                </xsl:when>

                <xsl:when test="modelid = 'ezechiel.model.linear.2comp.bolus'">
                     <a href="http://www.ezechiel.ch/?page_id=182#ezechiel.model.linear.2comp.bolus">Documentation</a>
                </xsl:when>
                <xsl:when test="modelid = 'ezechiel.model.linear.2comp.infu'">
                     <a href="http://www.ezechiel.ch/?page_id=182#ezechiel.model.linear.2comp.infu">Documentation</a>
                </xsl:when>
                <xsl:when test="modelid = 'ezechiel.model.linear.2comp.extra'">
                     <a href="http://www.ezechiel.ch/?page_id=182#ezechiel.model.linear.2comp.extra">Documentation</a>
                </xsl:when>

                <xsl:when test="modelid = 'ezechiel.model.linear.3comp.bolus'">
                     <a href="http://www.ezechiel.ch/?page_id=169#ezechiel.model.linear.3comp.bolus">Documentation</a>
                </xsl:when>
                <xsl:when test="modelid = 'ezechiel.model.linear.3comp.infu'">
                     <a href="http://www.ezechiel.ch/?page_id=169#ezechiel.model.linear.3comp.infu">Documentation</a>
                </xsl:when>
                <xsl:when test="modelid = 'ezechiel.model.linear.3comp.extra'">
                     <a href="http://www.ezechiel.ch/?page_id=169#ezechiel.model.linear.3comp.extra">Documentation</a>
                </xsl:when>



            </xsl:choose>
            
            </h5>
            <h5>ATC: <xsl:value-of select="atc" /></h5>
            <h5> <a href="{concat('http://www.whocc.no/atc_ddd_index/?code=',atc,'&amp;showdescription=yes')}">Link to whocc.no</a></h5>
            <h5>Brands: <xsl:for-each select="brands/brand"><xsl:value-of select="." /> | </xsl:for-each></h5>
            <h5>Domain: <xsl:value-of select="domainnames/name[@lang=$lang]" /></h5>
            <h5>Study: <xsl:value-of select="studynames/name[@lang=$lang]" /></h5>
            <p><xsl:value-of select="descs/desc[@lang=$lang]" /></p>
        </div>
    </xsl:template>


    <!-- DATA STYLING -->
    <xsl:template match="//drug">
        <a name="{generate-id(head/id)}" />
        <p><xsl:value-of select="comment" /></p>
        <xsl:apply-templates select="adme" />
        <xsl:apply-templates select="dosages" />
        <xsl:apply-templates select="conversions" />
        <xsl:apply-templates select="targets" />
        <xsl:apply-templates select="parameters" />
        <xsl:apply-templates select="errormodel" />
        <xsl:apply-templates select="covariates" />
        <xsl:apply-templates select="operations" />
        <xsl:apply-templates select="options" />
    </xsl:template>
    
    <xsl:template match="//conversions">
        <a name="conversions" />
        <div class="data">
            <h2>Conversions</h2>
            <table>
                <tr><td>Dose unit</td><td><xsl:value-of select="units/dose" /></td></tr>
                <tr><td>Concentration unit</td><td><xsl:value-of select="units/concentration" /></td></tr>
                <tr><td>Result unit</td><td><xsl:value-of select="units/result" /></td></tr>
                <tr><td>Factor of concentration</td><td><xsl:value-of select="factors/concentration" /></td></tr>
                <tr><td>Factor of result</td><td><xsl:value-of select="factors/result" /></td></tr>
                <tr><td>Molar mass</td><td><xsl:value-of select="molarmass" /></td></tr>
            </table>    

            <xsl:apply-templates select="comment" />
        </div>
    </xsl:template>
    
    <xsl:template match="//adme">
        <a name="adme" />
        <div class="data">
            <h2>Absorption, distribution, metabolism, and excretion</h2>
            <table>
                <tr><td>Intake: </td><td><xsl:value-of select="intake" /></td></tr>
                <tr><td>Distribution:  </td><td><xsl:value-of select="distribution" /> compartments</td></tr>
                <tr><td>Elimination:  </td><td><xsl:value-of select="elimination" /></td></tr>
            </table>
        </div>
    </xsl:template>
    
    
    <xsl:template match="//errormodel">
        <a name="errormodel" />
        <div class="data">
            <h2>Error model</h2>
            <table>
                <tr><th>Error</th><th>Value</th></tr>
                <tr><td>Additive</td><td><xsl:value-of select="additive" /></td></tr>
                <tr><td>Proportional</td><td><xsl:value-of select="proportional" /></td></tr>
            </table>
        
            <xsl:apply-templates select="comment" />   
        </div>
    </xsl:template>
    
    <xsl:template match="//comment">
        <table>
            <tr><td>Comment:</td><td align='left'><xsl:value-of select="." /></td></tr>
        </table>
    </xsl:template>

    <xsl:template match="//dosages">
        <a name="dosages" />
        <div class="data">
            <h2>Dosages</h2>
            <h4>Default dosage</h4>
            <table>
                <tr>
                    <th>Dose</th>
                    <th>Interval</th>
                    <th>Infusion</th></tr>
                <tr>
                    <td><xsl:value-of select="defaults/dose" /></td>
                    <td><xsl:value-of select="defaults/interval" /></td>
                    <td><xsl:value-of select="defaults/infusion" /></td>
                </tr>
            </table>
        
            <xsl:apply-templates select="doses" />
            <xsl:apply-templates select="intervals" />
            <xsl:apply-templates select="infusions" />
        </div>
    </xsl:template>

    <xsl:template match="//intervals">
        <h3>Intervals</h3>
        <table>
            <tr>
                <th>Unit</th>
                <th>Interval</th>
            </tr>
            <tr>
                <td><xsl:value-of select="@unit" /></td>
                <td>
                    <xsl:for-each select="interval"><xsl:value-of select="." /> | </xsl:for-each>
                </td>
            </tr>
        </table>
    </xsl:template>
        
    <xsl:template match="//infusions">
        <h3>Infusion times</h3>
        <table>
            <tr>
                <th>Unit</th>
                <th>Infusion time</th>
            </tr>
            <tr>
                <td><xsl:value-of select="@unit" /></td>
                <td>
                    <xsl:for-each select="infusion"><xsl:value-of select="." /> | </xsl:for-each>
                </td>
            </tr>
        </table>
    </xsl:template>

    
    <xsl:template match="//doses">
        <h3>Doses</h3>
        <table>
            <tr>
                <th>Unit</th>
                <th>Dose</th>
            </tr>
            <tr>
                <td><xsl:value-of select="@unit" /></td>
                <td>
                    <xsl:for-each select="dose"><xsl:value-of select="." /> | </xsl:for-each>
                </td>
            </tr>
        </table>
    </xsl:template>

    <xsl:template match="//targets">
        <a name="targets" />
        <div class="data">
            <h2>Targets Concentrations</h2>
            <table class="longtable">
                <tr>
                    <th>Type</th>
                    <th>Data type</th>
                    <th>Unit</th>
                    <th>Min</th>
                    <th>Best</th>
                    <th>Max</th>
                    <th class="comment">Comment</th>
                </tr>
                <xsl:for-each select="target">
                    <tr>
                        <td><xsl:value-of select="type" /></td>
                        <xsl:apply-templates select="concentrations" />
                        <td rowspan='2' class="comment"><xsl:value-of select="comment" /></td>
                    </tr>
                    <tr>
                        <xsl:apply-templates select="times" />
                    </tr>
                </xsl:for-each>
            </table>
        </div>
    </xsl:template>

    <xsl:template match="//concentrations">
        <td>Concentrations </td>
        <td><xsl:value-of select="@unit" /></td>
        <td>
            <xsl:if test="min/formula != ''">

                <table>
                    <tr>
                        <td>
                            <pre>default: <xsl:value-of select="min/value" />
else:
************************************
                                <xsl:value-of select="min/formula" />
                            </pre>
                        </td>
                    </tr>
                </table>
            </xsl:if>
        
            <xsl:if test="not(min/formula !='')">
                <xsl:value-of select="min/value" />
            </xsl:if>
        </td>

        <td>
            <xsl:if test="best/formula != ''">

                <table>
                    <tr>
                        <td>

                            <pre>default: <xsl:value-of select="best/value" />
else:
************************************
                                <xsl:value-of select="best/formula" />
                            </pre>
                        </td>
                    </tr>
                </table>
            </xsl:if>
            <xsl:if test="not(best/formula !='')">
                <xsl:value-of select="best/value" />
            </xsl:if>
        </td>

        <td>
            <xsl:if test="max/formula != ''">

                <table>
                    <tr>
                        <td>

                            <pre>default: <xsl:value-of select="max/value" />
else:
************************************
                                <xsl:value-of select="max/formula" />
                            </pre>
                        </td>
                    </tr>
                </table>
            </xsl:if>
            <xsl:if test="not(max/formula !='')">
                <xsl:value-of select="max/value" />
            </xsl:if>
        </td>

    </xsl:template>

    <xsl:template match="//times">
        <td></td>
        <td>Times: </td>
        <td><xsl:value-of select="@unit" /></td>
        <td><apply-templates select="min" /> <xsl:value-of select="min" /></td>
        <td><xsl:value-of select="best" /></td>
        <td><xsl:value-of select="max" /></td>
    </xsl:template>



    <xsl:template match="//parameters">
        <a name="parameters" />
        <div class="data">
        <h2>Parameters</h2>
        <table class="longtable">
            <tr>
                <th>Name</th><th>Unit</th><th>Value</th>
                <th>Dev. std</th><th>Dev. Prop</th>
                <th>Min</th><th>Max</th><th>Step</th>
                <th class="comment">Comment</th>
            </tr>

            <xsl:for-each select="parameter">
                <tr>
                    <td><xsl:value-of select="id" /></td>
                    <td><xsl:value-of select="unit" /></td>
                    <td><xsl:value-of select="value" /></td>
                    <td><xsl:value-of select="bsv/additive" /></td>
                    <td><xsl:value-of select="bsv/proportional" /></td>
                    <td><xsl:value-of select="min" /></td>
                    <td><xsl:value-of select="max" /></td>
                    <td><xsl:value-of select="step" /></td>
                    <td class="comment"><xsl:value-of select="comment" /></td>
                </tr>
            </xsl:for-each>
        </table>
        <br />
        <xsl:apply-templates select="//correlations" />
        </div>
    </xsl:template>

    <xsl:template match="//covariates">
        <a name="covariates" />
        <div class="data">
            <h2>Covariates</h2>
            <table class="longtable">
                <tr>
                    <th>ID</th><th>Name</th><th>Default Value</th><th class="comment">Comment</th>
                </tr>
                <xsl:for-each select="covariate">
                    <tr>
                        <td><xsl:value-of select="id" /></td>
                        <td><xsl:value-of select="names/name[@lang=$lang]" /></td>
                        <td><xsl:value-of select="defaultvalue" /></td>
                        <td class="comment"><xsl:value-of select="comment" /></td>
                    </tr>
                </xsl:for-each>
            </table>
        </div>
    </xsl:template>


    <xsl:template match="//correlations">
        <h3>Correlations</h3>
        <table>
            <tr>
                <th>Parameter 1</th><th>Parameter 2</th><th>Correlation</th>
            </tr>
            <xsl:for-each select="correlation">
                <tr>
                    <td><xsl:value-of select="param1" /></td>
                    <td><xsl:value-of select="param2" /></td>
                    <td><xsl:value-of select="value" /></td>
                </tr>    
            </xsl:for-each>

        </table>
    </xsl:template>

    <xsl:template match="//operations">
        <a name="operations" />
        <div class="data">
            <h2>Operations</h2>

            <xsl:for-each select="operation">
                <p class="opParameter">
                    <strong>Output parameter</strong>   
                    <xsl:value-of select="parameter" />
                </p>
                <p><xsl:value-of select="comment" /></p>
                <pre>
                    <xsl:value-of select="formula" />
                </pre>
    
    
            </xsl:for-each>

        </div>
    </xsl:template>





    <!-- FILE HISTORY STYLING -->
    <xsl:template match="//history">
        <a name="history" />
        <div class="history">
            <h2>File creator</h2>
            <table class="longtable">
                <tr>
                    <th>Author</th>
                    <th>Date</th>
                    <th>Comment</th>
                </tr>
                <xsl:for-each select="creator">
                    <tr>
                        <xsl:attribute name="class"><xsl:value-of select="local-name()" /></xsl:attribute>
                        <td><a><xsl:attribute name="href">mailto:<xsl:value-of select="email" /></xsl:attribute>
                            <xsl:value-of select="name" /></a></td>
                        <td><xsl:value-of select="date" /></td>
                        <td><xsl:value-of select="comment" /></td>
                    </tr>
                </xsl:for-each>
            </table>
            
            <br />
            <br />
            
            <h2>Revisions</h2>
            <table class="longtable">
                <tr>
                    <th>Author</th>
                    <th>Date</th>
                    <th>Comment</th>
                </tr>
                <xsl:for-each select="revisions/revision">
                    <tr>
                        <xsl:attribute name="class"><xsl:value-of select="local-name()" /></xsl:attribute>
                        <td><a><xsl:attribute name="href">mailto:<xsl:value-of select="email" /></xsl:attribute>
                            <xsl:value-of select="name" /></a></td>
                        <td><xsl:value-of select="date" /></td>
                        <td><xsl:value-of select="comment" /></td>
                    </tr>
                </xsl:for-each>
            </table>    
        </div>    
    </xsl:template>

    <!-- Not used -->
    <xsl:template match="//options">
        <h3>Model Options</h3>
        <table>
            <tr>
                <th>name</th><th>value</th><th class="comment">Comment</th>
            </tr>
            <xsl:for-each select="option">
                <tr>
                    <td><xsl:value-of select="id" /></td>
                    <td><xsl:value-of select="value" /></td>
                    <td class="comment"><xsl:value-of select="comment" /></td>
                </tr>
            </xsl:for-each>
        </table>
    </xsl:template>

</xsl:stylesheet>
