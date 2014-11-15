# wip - xlsdump

xsldump is a re-working of, **but substantially based on**, the xls2csv that is available within http://libxls.sourceforge.net which dumps all of the sheets in a spreadsheet to disk at a specific location. The original only dumped the specified sheet.

It is intended that xsldump will watch a folder for new .xls files, and generate new folders containing the .csv version of each sheet.

## Build

```
# dependencies
svn checkout svn://svn.code.sf.net/p/libxls/code/trunk libxls-code
cd libxls-code
./configure --prefix=/usr/local;
make;
sudo make install

# app
git clone git@github.com:rossjones/xlsdump.git
cd xlsdump/
make
```
