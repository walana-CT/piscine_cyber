<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
  <xsl:output method="xml" encoding="UTF-8" indent="yes"
	      doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
	      doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"/>
  <xsl:template match="/">

<html>
  <head>
    <link rel="stylesheet" href="asciidoc.css" type="text/css" />
    <link rel="stylesheet" href="layout.css" type="text/css" />
    <xsl:copy-of select="//head/*" />
  </head>
  <body>
    <div id="layout-menu-box">
      <div id="layout-menu">
	<div>&#187;<a href="index.html">Home</a></div>
	<div>&#187;<a href="NEWS.html">News</a></div>
	<div>&#187;<a href="download.html">Download</a></div>
	<div>&#187;<a href="docs.html">Documentation</a></div>
	<div>&#160;&#160;&#160;&#160;&#160;&#187;<a href="oathtool.1.html">oathtool(1)</a></div>
	<div>&#160;&#160;&#160;&#160;&#160;&#187;<a href="pskctool.1.html">pskctool(1)</a></div>
	<div>&#160;&#160;&#160;&#160;&#160;&#187;<a href="liboath-api/liboath-oath.h.html">Liboath&#160;API</a></div>
	<div>&#160;&#160;&#160;&#160;&#160;&#187;<a href="libpskc-api/pskc-tutorial.html">PSKC Tutorial</a></div>
	<div>&#160;&#160;&#160;&#160;&#160;&#187;<a href="libpskc-api/pskc-reference.html">Libpskc&#160;API</a></div>
	<div>&#160;&#160;&#160;&#160;&#160;&#187;<a href="pam_oath.html">pam_oath</a></div>
	<div>&#187;<a href="contrib.html">Contribute</a></div>
      </div>
    </div>
    <div id="layout-content-box">
      <div id="layout-banner">
	<div id="layout-title">OATH Toolkit</div>
	<div id="layout-description">One-time password components</div>
      </div>
      <div id="layout-content">
	<div id="header">
	  <xsl:copy-of select="//div[@id='header']/*" />
	</div>
	<div id="content">
	    <xsl:choose>
		<xsl:when test="//div[@id='content']">
		    <xsl:copy-of select="//div[@id='content']/*" />
		</xsl:when>
		<xsl:otherwise>
		    <xsl:copy-of select="//body/*" />
		</xsl:otherwise>
	    </xsl:choose>
	</div>
	<div id="footer">
	  <div id="footer-badges">
	    <a href="https://validator.w3.org/check?uri=referer">
	      <img style="border:0;width:88px;height:31px"
		   src="https://www.w3.org/Icons/valid-xhtml10"
		   alt="Valid XHTML 1.0" height="31" width="88" />
	    </a>
	    <a href="https://jigsaw.w3.org/css-validator/check/referer">
	      <img style="border:0;width:88px;height:31px"
		   src="https://jigsaw.w3.org/css-validator/images/vcss"
		   alt="Valid CSS!" />
	    </a>
	  </div>
	</div>
      </div>
    </div>
  </body>
</html>

  </xsl:template>
</xsl:stylesheet>
