ack "Item::get\('(.*?)'\)" --output='$1' \
  | cut -d: -f3 \
  | sort -u \
  > items.txt
