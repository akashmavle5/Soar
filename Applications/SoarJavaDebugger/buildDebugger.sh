#!/bin/sh
# Builds the debugger
SOARLIB="../soar-library"

for file in `find . -name "*.class"`
do
  rm -f $file
done

if [[ `uname -s` == "Darwin" ]]
then
  if ! javac -classpath .:${SOARLIB}/swt-osx.jar:${SOARLIB}/sml.jar -sourcepath . debugger/Application.java; then
    echo "Build failed."
    exit 1;
  fi
  jar cfm ${SOARLIB}/SoarJavaDebugger.jar JarManifest-osx .
else
  if ! javac -classpath .:${SOARLIB}/swt-motif.jar:${SOARLIB}/sml.jar -sourcepath . debugger/Application.java; then
    echo "Build failed."
    exit 1;
  fi
  jar cfm ${SOARLIB}/SoarJavaDebugger.jar JarManifest-motif .
fi

if [[ `uname -s` == "Darwin" ]]
then
    echo "on Mac OS X, building application package for SoarDebugger..."

    APP_PATH=$SOARLIB/SoarDebugger.app/Contents
    mkdir -p $APP_PATH/MacOS
    mkdir -p $APP_PATH/Resources/Java

    cp SoarDebugger.plist $APP_PATH/Info.plist
    cp $SOARLIB/icons/testapp.icns $APP_PATH/Resources
    cp $SOARLIB/SoarJavaDebugger.jar $APP_PATH/Resources/Java
    cp /System/Library/Frameworks/JavaVM.framework/Resources/MacOS/JavaApplicationStub $APP_PATH/MacOS
    chmod a+x $APP_PATH/MacOS/JavaApplicationStub
fi
