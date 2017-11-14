ack 'this->locations\["(.*?)"\]->setItem' --output='$1' | cut -d: -f3 | sort -u > locations.txt
