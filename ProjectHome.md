**SplashKit helps you to build rich HTML and JavaScript applications, splash screens and launchers that can be easily deployed and run from CD, DVD and USB drives.**

http://splashkit.googlecode.com/hg/assets/Overview.PNG

## What's it for? ##

Suppose your company is sponsoring a conference. You want to hand out DVD's or USB drives with some promotional material - perhaps a short video, some documents, and a trial installer for your software.

To provide a nice experience, you want some kind of launch screen to appear when the DVD/USB drive is inserted.

How will you build this launch screen? Users might not have Java, .NET or Flex runtimes installed, and they might be annoyed at having to run an installer. The answer is plain old HTML, and SplashKit.

## Enter SplashKit ##

With SplashKit, you can build your welcome experience using basic HTML and JavaScript. The SplashKit executable acts as the gateway - an embedded browser with a nice icon that users can double-click.

SplashKit provides:

  * An EXE with [Webkit](http://webkit.org/) embedded, for rendering your HTML/JavaScript
  * Special [JavaScript extensions](JSExtensions.md) for launching installers and checking for installed components

## Getting started ##

I have some guides to show how to create SplashKit applications depending on how you plan to distribute your application.

  * [Creating a DVD/CD-ROM with a SplashKit launcher](InstallerDVD.md)
  * [Creating a USB drive with a SplashKit launcher](InstallerUSB.md)
  * [Creating an internet-distributed application (.ZIP file)](InstallerZIP.md)

## FAQ ##

**How does it work?**

SplashKit is an EXE written in unmanaged C++, and uses the [WebKit](http://webkit.org/) rendering engine (the same used in Safari/Google Chrome). Everything users need to run your SplashKit application will be in the binaries - you won't need them to install anything.

**Why shouldn't I just use [HTA](http://en.wikipedia.org/wiki/HTML_Application)'s?**

SplashKit takes many of the ideas of HTA's (which use Internet Explorer), but uses WebKit, which has much better support for HTML5, including video rendering. You can also use SplashKit's [JavaScript extensions](JSExtensions.md) to automate common launcher tasks, like checking for installed software, or launching an explorer or EXE.

**What platforms are supported?**

SplashKit will work on Windows XP (any version), Windows Vista, Windows 7, and Windows Server 2003 and 2008. If you're a Linux/OSX fellow, I'd love help making it fully cross platform.