/* -*- Mode: Java; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

  var appCore = null;
  var prefwindow = null;
  var appCoreName = "";
  var defaultStatus = "default status text";
  var debugSecurity = false; // Set this true to enable Security chrome testing.

  function Startup()
  {
    dump("Doing Startup...\n");
    dump("Creating browser app core\n");
    appCore = new BrowserAppCore();
    if (appCore != null) {
      dump("BrowserAppCore has been created.\n");
	  appCoreName = "BrowserAppCore." + ( new Date() ).getTime().toString();
	  appCore.Init( appCoreName );
	  appCore.setWebShellWindow(window);
	  appCore.setToolbarWindow(window);
	  tryToSetContentWindow();
    }
  }

  function onLoadWithArgs() {
    // See if Startup has been run.
    if ( appCore ) {
        // See if load in progress (loading default page).
        if ( document.getElementById("Browser:Throbber").getAttribute("busy") == "true" ) {
            dump( "Stopping load of default initial page\n" );
            appCore.stop();
        }
        dump( "Loading page specified on ShowWindowWithArgs\n" );
        appCore.loadInitialPage();
    } else {
        // onLoad handler timing is not correct yet.
        dump( "onLoadWithArgs not needed yet\n" );
    }
  }

  function tryToSetContentWindow() {
    if ( window.frames[0].frames[1] ) {
        dump("Setting content window\n");
        appCore.setContentWindow( window.frames[0].frames[1] );
        // Have browser app core load appropriate initial page.

        var pref = Components.classes['component://netscape/preferences'];

        // if all else fails, use trusty "about:" as the start page
        var startpage = "about:";  
        if (pref) {
          pref = pref.getService();
        }
        if (pref) {
          pref = pref.QueryInterface(Components.interfaces.nsIPref);
        }
        if (pref) {
          // from mozilla/modules/libpref/src/init/all.js
          // 0 = blank 
          // 1 = home (browser.startup.homepage)
          // 2 = last 
          // 3 = splash (browser.startup.splash)
          choice = pref.GetIntPref("browser.startup.page");
	  switch (choice) {
		case 0:
            		startpage = "about:blank";
      			break;
		case 1:
            		startpage = pref.CopyCharPref("browser.startup.homepage");
      			break;
		case 2:
            		var history = Components.classes['component://netscape/browser/global-history'];
			if (history) {
               			history = history.getService();
	    		}
	    		if (history) {
              			history = history.QueryInterface(Components.interfaces.nsIGlobalHistory);
	    		}
	    		if (history) {
				startpage = history.GetLastPageVisted();
	    		}
      			break;
		case 3:
            		startpage = pref.CopyCharPref("browser.startup.splash");
      			break;
   		default:
            		startpage = "about:";
	  }
        }
        document.getElementById("args").setAttribute("value", startpage);
        appCore.loadInitialPage();
    } else {
        // Try again.
        dump("Scheduling later attempt to set content window\n");
        window.setTimeout( "tryToSetContentWindow()", 100 );
    }
  }

  function Translate(src, dest)
  {
	var service = "http://levis.alis.com:8081";
	service += "?AlisSourceLang=" + src;
	service += "&AlisTargetLang=" + dest;
	service += "&AlisMTEngine=SSI";
	service += "&AlisTargetURI=" + window.frames[0].frames[1].location.href;
	window.frames[0].frames[1].location.href = service;
  }

  function RefreshUrlbar()
  {
   //Refresh the urlbar bar
    document.getElementById('urlbar').value = window.frames[0].frames[1].location.href;
  }

  function BrowserBack()
  {
     // Get a handle to the back-button
     var bb = document.getElementById("canGoBack");
     // If the button is disabled, don't bother calling in to Appcore
     if ( (bb.getAttribute("disabled")) == "true" ) 
        return;

    if (appCore != null) {
      dump("Going Back\n");
      appCore.back();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserForward()
  {
     // Get a handle to the back-button
     var fb = document.getElementById("canGoForward");
     // If the button is disabled, don't bother calling in to Appcore
     if ( (fb.getAttribute("disabled")) == "true" ) 
        return;

    if (appCore != null) {
      dump("Going Forward\n");
      appCore.forward();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserSetForward()
  {
     var forwardBElem = document.getElementById("canGoForward");
     if (!forwardBElem) {
	dump("Couldn't obtain handle to forward Broarcast element\n");
	return;
	}

     var canForward = forwardBElem.getAttribute("disabled");
     var fb = document.getElementById("forward-button");
     
     if (!fb) {
	dump("Could not obtain handle to forward button\n");
	return;
     }
	
     // Enable/Disable the Forward button      
     if (canForward == "true")  {
        fb.setAttribute("disabled", "true");
     }
     else {
        fb.setAttribute("disabled", "");
     }
        
     // Enable/Disable the Forward menu
     var fm = document.getElementById("menuitem-forward");
     if (!fm) {
       dump("Couldn't obtain menu item Forward\n");
       return;
     }

     // Enable/Disable the Forward Menuitem
     if (canForward == "true")  {
        fm.setAttribute("disabled", "true");
     }
     else {
	dump("Setting forward menu item enabled\n");
        fm.setAttribute("disabled", "");
     }
    
  }

  function BrowserCanStop() {
    var stop = document.getElementById("canStop");
    if ( stop ) {
        var stopDisabled = stop.getAttribute("disabled");
        var stopButton   = document.getElementById( "stop-button" );
        if ( stopButton ) {
            if ( stopDisabled == "true") {
                stopButton.setAttribute( "disabled", "true" );
            } else {
                stopButton.setAttribute( "disabled", "" );
            }
        }
    }
  }

  function BrowserStop() {
     // Get a handle to the "canStop" broadcast id
     var stopBElem = document.getElementById("canStop");
     if (!stopBElem) {
        dump("Couldn't obtain handle to stop Broadcast element\n");
        return;
	 }

     var canStop = stopBElem.getAttribute("disabled");
     var sb = document.getElementById("stop-button");
     
     if (!sb) {
    	dump("Could not obtain handle to stop button\n");
	    return;
     }

     // If the stop button is currently disabled, just return
     if ((sb.getAttribute("disabled")) == "true") {
	    return;
     }
	
     //Stop button has just been pressed. Disable it. 
     sb.setAttribute("disabled", "true");

     // Get a handle to the stop menu item.
     var sm = document.getElementById("menuitem-stop");
     if (!sm) {
       dump("Couldn't obtain menu item Stop\n");
     } else {
       // Disable the stop menu-item.
       sm.setAttribute("disabled", "true");
     }
  
     //Call in to BrowserAppcore to stop the current loading
     if (appCore != null) {
        dump("Going to Stop\n");
        appCore.stop();
     } else {
        dump("BrowserAppCore has not been created!\n");
     }
  }

  function BrowserSetBack()
  {
     var backBElem = document.getElementById("canGoBack");
     if (!backBElem) {
	dump("Couldn't obtain handle to back Broadcast element\n");
	return;
	}

     var canBack = backBElem.getAttribute("disabled");
     var bb = document.getElementById("back-button");
     
     if (!bb) {
	dump("Could not obtain handle to back button\n");
	return;
     }
	
     // Enable/Disable the Back button      
     if (canBack == "true")  {
        bb.setAttribute("disabled", "true");
     }
     else {
        bb.setAttribute("disabled", "");
     }
        
     // Enable/Disable the Back menu
     var bm = document.getElementById("menuitem-back");
     if (!bm) {
       dump("Couldn't obtain menu item Back\n");
       return;
     }

     // Enable/Disable the Back Menuitem
     if (canBack == "true")  {
        bm.setAttribute("disabled", "true");
     }
     else {
	dump("Setting Back menuitem to enabled\n");
        bm.setAttribute("disabled", "");
     }
    
     
  }

  function BrowserHome()
  {
   window.frames[0].frames[1].home();
   RefreshUrlbar();
  }

  function OpenBookmarkURL(node)
  {
    if (node.getAttribute('container') == "true") {
      return false;
    }
    url = node.getAttribute('id');

    // Ignore "NC:" urls.
    if (url.substring(0, 3) == "NC:") {
      return false;
    }

    window.frames[0].frames[1].location.href = url;
    RefreshUrlbar();
  }

  function BrowserNewWindow()
  {
    if (appCore != null) {
	    dump("Opening New Window\n");
      appCore.newWindow();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserNewEditorWindow()
  {
    core = XPAppCoresManager.Find("toolkitCore");
    if ( !core ) {
        core = new ToolkitCore();
        if ( core ) {
            core.Init("toolkitCore");
        }
    }
    if ( core ) {
        core.ShowWindowWithArgs( "chrome://editor/content", window, "chrome://editor/content/EditorInitPage.html" );
    } else {
        dump("Error; can't create toolkitCore\n");
    }
  }
  
  function BrowserNewTextEditorWindow()
  {
    core = XPAppCoresManager.Find("toolkitCore");
    if ( !core ) {
        core = new ToolkitCore();
        if ( core ) {
            core.Init("toolkitCore");
        }
    }
    if ( core ) {
        core.ShowWindowWithArgs( "chrome://editor/content/TextEditorAppShell.xul", window, "chrome://editor/content/EditorInitPagePlain.html" );
    } else {
        dump("Error; can't create toolkitCore\n");
    }
  }
  
  function BrowserEditPage(url)
  {
    core = XPAppCoresManager.Find("toolkitCore");
    if ( !core ) {
        core = new ToolkitCore();
        if ( core ) {
            core.Init("toolkitCore");
        }
    }
    if ( core ) {
        core.ShowWindowWithArgs( "chrome://editor/content", window, url);
    } else {
        dump("Error; can't create toolkitCore\n");
    }
  }
  
  function BrowserOpenWindow()
  {
    core = XPAppCoresManager.Find("toolkitCore");
    if ( !core ) {
        core = new ToolkitCore();
        if ( core ) {
            core.Init("toolkitCore");
        }
    }
    if ( core ) {
        //core.ShowWindowWithArgs( "chrome:/navigator/content/openLocation.xul", window, appCoreName );
        var name = appCoreName.replace( /\./, /\_/ );
        // Note: Use width/height one less than actual so resizing occurs.
        //       This bypasses bug whereby dialog contents don't appear
        //       till the dialog is resized.
        window.openDialog( "chrome:/navigator/chrome/openLocation.xul", name+"_openLocation", "chrome,width=419,height=189", appCoreName );
    } else {
        dump("Error; can't create toolkitCore\n");
    }
  }

  function OpenFile(url) {
    // This is invoked from the browser app core.
    core = XPAppCoresManager.Find("toolkitCore");
    if ( !core ) {
        core = new ToolkitCore();
        if ( core ) {
            core.Init("toolkitCore");
        }
    }
    if ( core ) {
        core.ShowWindowWithArgs( "chrome://navigator/content/navigator.xul", window, url );
    } else {
        dump("Error; can't create toolkitCore\n");
    }
  }

  function BrowserCopy()
  {
    if (appCore != null) {
	    dump("Copying\n");
      appCore.copy();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }


  function BrowserAddBookmark(url,title)
  {
    var bmks = Components.classes["component://netscape/browser/bookmarks-service"].getService();
    bmks = bmks.QueryInterface(Components.interfaces.nsIBookmarksService);
    bmks.AddBookmark(url, title);
  }

  function BrowserEditBookmarks()
  {
    var toolkitCore = XPAppCoresManager.Find("toolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("toolkitCore");
      }
    }
    if (toolkitCore) {
      toolkitCore.ShowWindow("resource://res/samples/bookmarks.xul",window);
    }
  }

  function OpenHistoryView()
  {
    var toolkitCore = XPAppCoresManager.Find("toolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("toolkitCore");
      }
    }
    if (toolkitCore) {
      toolkitCore.ShowWindow("resource://res/samples/history.xul",window);
    }
  }

  function BrowserPrintPreview()
  {
    // Borrowing this method to show how to 
    // dynamically change icons
    dump("BrowserPrintPreview\n");
    if (appCore != null) {
	    dump("Changing Icons\n");
      appCore.printPreview();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserPrint()
  {
    // Borrowing this method to show how to 
    // dynamically change icons
    if (appCore != null) {
      appCore.print();
    }
  }

  function BrowserSetDefaultCharacterSet(aCharset)
  {
    if (appCore != null) {
      appCore.SetDocumentCharset(aCharset);
      window.frames[0].frames[1].location.reload();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserClose()
  {
    dump("BrowserClose\n");
  // Currently window.close doesn't work unless the window was opened from JS
  //	 window.close();
  
   core = XPAppCoresManager.Find("toolkitCore");
    if ( !core ) {
        core = new ToolkitCore();
        if ( core ) {
            core.Init("toolkitCore");
        }
    }
    if ( core ) {
        core.CloseWindow( window );
    } else {
        dump("Error can't create toolkitCore\n");
    }
  }

  function BrowserExit()
  {
    if (appCore != null) {
	    dump("Exiting\n");
      appCore.exit();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserFind() {
    if (appCore != null) {
        appCore.find();      
    } else {
        dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserFindAgain() {
    if (appCore != null) {
        appCore.findNext();      
    } else {
        dump("BrowserAppCore has not been created!\n");
    }
  }

  function BrowserLoadURL()
  {
	if (appCore == null)
	{
		dump("BrowserAppCore has not been initialized\n");
		return;
	}

    // rjc: added support for URL shortcuts (3/30/1999)
    try {
      var bmks = Components.classes["component://netscape/browser/bookmarks-service"].getService();
      bmks = bmks.QueryInterface(Components.interfaces.nsIBookmarksService);

      var shortcutURL = bmks.FindShortcut(document.getElementById('urlbar').value);

      dump("FindShortcut: in='" + document.getElementById('urlbar').value + "'  out='" + shortcutURL + "'\n");

      if ((shortcutURL != null) && (shortcutURL != "")) {
        document.getElementById('urlbar').value = shortcutURL;
      }
	}
    catch (ex) {
      // stifle any exceptions so we're sure to load the URL.
    }

	appCore.loadUrl(document.getElementById('urlbar').value);
      
  }

  function WalletEditor()
  {
    if (appCore != null) {
      dump("Wallet Editor\n");
      appCore.walletEditor(window);
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function WalletSafeFillin()
  {
    if (appCore != null) {
      dump("Wallet Safe Fillin\n");
      appCore.walletPreview(window, window.frames[0].frames[1]);
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function WalletChangePassword()
  {
    if (appCore != null) {
      dump("Wallet Change Password\n");
      appCore.walletChangePassword();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }


  function WalletQuickFillin()
  {
    if (appCore != null) {
      dump("Wallet Quick Fillin\n");
      appCore.walletQuickFillin(window.frames[0].frames[1]);
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function WalletSamples()
  {
    if (appCore != null) {
      dump("Wallet Samples\n");
      appCore.walletSamples();
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function SignonViewer()
  {
    if (appCore != null) {
      dump("Signon Viewer\n");
      appCore.signonViewer(window);
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function CookieViewer()
  {
    if (appCore != null) {
      dump("Cookie Viewer\n");
      appCore.cookieViewer(window);
    } else {
      dump("BrowserAppCore has not been created!\n");
    }
  }

  function OpenMessenger()
  {
    var toolkitCore = XPAppCoresManager.Find("ToolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("ToolkitCore");
      }
    }
    if (toolkitCore) {
      toolkitCore.ShowWindow("chrome://messenger/content/",
                             window);
    }
  }

  function OpenAddressbook()
  {
    var toolkitCore = XPAppCoresManager.Find("ToolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("ToolkitCore");
      }
    }
    if (toolkitCore) {
      toolkitCore.ShowWindow("chrome://addressbook/content/",
                             window);
    }
  }

  function MsgNewMessage()
  {
    var toolkitCore = XPAppCoresManager.Find("ToolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("ToolkitCore");
      }
    }
    if (toolkitCore) {
      toolkitCore.ShowWindow("chrome://messengercompose/content/",
                             window);
    }
  }
  
  function DoPreferences()
  {
    if (!prefwindow)
    {
    	prefwindow = Components.classes['component://netscape/prefwindow'].createInstance(Components.interfaces.nsIPrefWindow);
	}
    prefwindow.showWindow("navigator.js", window, "chrome://pref/content/pref-appearance.xul");
  }

  function BrowserViewSource()
  {
    var toolkitCore = XPAppCoresManager.Find("ToolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("ToolkitCore");
      }
    }
    if (toolkitCore) {
      var url = window.frames[0].frames[1].location;
      dump("Opening view of source for" + url + "\n");
      toolkitCore.ShowWindowWithArgs("chrome:/navigator/content/viewSource.xul", window, url);
    }
  }

  function OpenEditor()
  {
    var toolkitCore = XPAppCoresManager.Find("ToolkitCore");
    if (!toolkitCore) {
      toolkitCore = new ToolkitCore();
      if (toolkitCore) {
        toolkitCore.Init("ToolkitCore");
      }
    }
    if (toolkitCore) {
      toolkitCore.ShowWindowWithArgs("chrome://editor/content/EditorAppShell.xul",window,"chrome://editor/content/EditorInitPage.html");
    }
  }
        var bindCount = 0;
        function onStatus() {
            var status = document.getElementById("Browser:Status");
            if ( status ) {
                var text = status.getAttribute("value");
                if ( text == "" ) {
                    text = defaultStatus;
                }
                var statusText = document.getElementById("statusText");
                if ( statusText ) {
                    statusText.setAttribute( "value", text );
                }
            } else {
                dump("Can't find status broadcaster!\n");
            }
        }

        function onSecurity() {
            var security = document.getElementById("Browser:Security");
            var indicator = document.getElementById("security-box");
            var icon = document.getElementById("security-button");
 
            if ( security.getAttribute("secure") == "true" ) {
                indicator.setAttribute("class","secure");
                icon.setAttribute("class","secure");
            } else {
                indicator.setAttribute("class","insecure");
                icon.setAttribute("class","insecure");
            }
        }

        function securityOn() {
            // Set debugSecurity (at top of file) to turn this code on.
            if ( !debugSecurity ) {
                return;
            }

            var security = document.getElementById("Browser:Security");
            if ( security.getAttribute("secure") == "false" ) {
                security.setAttribute("secure","true");
            }
        }
        function securityOff() {
            var security = document.getElementById("Browser:Security");
            if ( security.getAttribute("secure") == "true" ) {
                security.setAttribute("secure","false");
            }
        }
        function doTests() {
            // Turn security on.
            securityOn();
        }
		var startTime = 0;
        function onProgress() {
            var throbber = document.getElementById("Browser:Throbber");
            var meter    = document.getElementById("Browser:LoadingProgress");
            if ( throbber && meter ) {
                var busy = throbber.getAttribute("busy");
                if ( busy == "true" ) {
                    mode = "undetermined";
					if ( !startTime ) {
						startTime = (new Date()).getTime();
					}
                } else {
                    mode = "normal";
                }
                meter.setAttribute("mode",mode);
                if ( mode == "normal" ) {
                    var status = document.getElementById("Browser:Status");
                    if ( status ) {
						var elapsed = ( (new Date()).getTime() - startTime ) / 1000;
						var msg = "Document: Done (" + elapsed + " secs)";
						dump( msg + "\n" );
                        status.setAttribute("value",msg);
                        defaultStatus = msg;
                    }
					startTime = 0;
                }
            }
        }
        function dumpProgress() {
            var broadcaster = document.getElementById("Browser:LoadingProgress");
            var meter       = document.getElementById("meter");
            dump( "bindCount=" + bindCount + "\n" );
            dump( "broadcaster mode=" + broadcaster.getAttribute("mode") + "\n" );
            dump( "broadcaster value=" + broadcaster.getAttribute("value") + "\n" );
            dump( "meter mode=" + meter.getAttribute("mode") + "\n" );
            dump( "meter value=" + meter.getAttribute("value") + "\n" );
        }

function ShowWindowFromResource( node )
{
	var windowManager = Components.classes['component://netscape/rdf/datasource?name=window-mediator'].getService();
	dump("got window Manager \n");
	var	windowManagerInterface = windowManager.QueryInterface( Components.interfaces.nsIWindowMediator);
    dump("got interface \n");
    
    var desiredWindow = null;
    var url = node.getAttribute('id');
    dump( url +" finding \n" );
	desiredWindow = windowManagerInterface.GetWindowForResource( url );
	dump( "got window \n");
	if ( desiredWindow )
	{
		dump("focusing \n");
		desiredWindow.focus();
	}
}
