filelist=`find src -type f`
echo ${filelist}

po_file=./locale/ru/Kredo.po
if [ -f "$po_file" ]; then
	echo "updating $po_file"
else
	echo "initializing $po_file"
	touch $po_file
fi

xgettext -j -C -n -k_ -o $po_file $filelist
sed -i 's/charset=CHARSET/charset=UTF-8/g' $po_file
