#!/bin/bash

# options="content-type:application/x-www-form-urlencoded\r\ntransfer-encoding: chunked"
# options="content-type:application/x-www-form-urlencoded\r\ncontent-length:9"
# options="content-type:application/x-www-form-urlencoded"

# (echo -e "$req\r\n$host\r\n$options\r\n\r\n5\r\ndata=\r\n4\r\nlala\r\n0\r\n") | nc lala.42.fr 8080
# (echo -e "$req\r\n$host\r\n$options\r\n\r\ndata=lala\r\n") | nc lala.42.fr 8080


req="POST /post.php HTTP/1.1"
host="Host: lala.42.fr"
option="content-type:application/x-www-form-urlencoded"

# (echo -e "$req\r\n$host\r\n$option\r\nTransfer-Encoding: chunked\r\n\r\n"; \
# echo -e "4\r\ndata\r\n"; \
# echo -e "1\r\n=\r\n"; \
# echo -e "4\r\nWiki\r\n"; \
# echo -e "6\r\npedia0\r\n"; \
# echo -e "b\r\n in chunks.\r\n"; \
# echo -e "0\r\n\r\n") | nc lala.42.fr 8080

chunk1="Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi commodo odio ut ipsum porta, vel mattis odio ultricies. Vivamus pretium condimentum sapien scelerisque fermentum. Sed vitae erat eget tellus gravida feugiat. Nunc dictum ultrices nibh ut placerat. In massa turpis, fringilla eget congue bibendum, scelerisque eu nulla. Etiam libero dolor, ultricies eget hendrerit vel, molestie sed arcu. Nullam egestas, erat vitae rutrum condimentum, magna nisl elementum odio, vel laoreet mi lacus sit amet nibh. Cras aliquam dolor ac rhoncus sodales. Duis a dignissim dui. Cras aliquam dictum elit, ut cursus lorem convallis non. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus."
chunk2="Vestibulum sed tempor libero, eu feugiat eros. Vivamus id odio vitae tellus feugiat ornare vel pretium magna. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Praesent ac augue velit. Suspendisse potenti. Nulla dignissim ex vel mi consequat dignissim. In quis posuere leo. Praesent egestas enim sed massa accumsan finibus. In congue ante et ex commodo euismod."
chunk3="Curabitur vitae felis quis mi tristique pretium. Maecenas condimentum neque turpis. Sed egestas, dolor sit amet accumsan porta, est ex feugiat enim, vel lobortis tellus erat nec metus. Nunc tristique sed orci a ornare. Mauris eu sodales metus. Nulla facilisi. Praesent consectetur facilisis porttitor. Curabitur eu est euismod, faucibus sem eget, accumsan odio. Nam imperdiet magna quis porttitor dignissim. Suspendisse fringilla urna ipsum, commodo suscipit sem pharetra eget. In nibh justo, luctus at aliquam nec, auctor nec ante. Maecenas aliquam aliquet massa, et feugiat dolor luctus nec. Nam posuere imperdiet arcu, ut volutpat tellus venenatis ut. Donec posuere tincidunt neque, ac dapibus ex aliquam eu. Proin at ullamcorper nulla, vel condimentum erat. Mauris eleifend risus quis augue commodo posuere."
chunk4="Morbi iaculis quis risus eget iaculis. Aenean nisi quam, placerat sit amet lobortis id, efficitur vitae risus. Aenean augue risus, mollis at sapien ut, iaculis pellentesque sem. Nulla vulputate bibendum diam vitae sollicitudin. Maecenas efficitur consectetur purus, sed condimentum lectus. Suspendisse laoreet, lorem placerat sollicitudin laoreet, quam ipsum hendrerit diam, a placerat neque massa nec purus. Sed ac gravida nulla. Phasellus tincidunt urna orci, in luctus ex pretium in. Praesent auctor orci mi, sit amet tempor nunc bibendum sit amet. Proin lacinia diam eget venenatis auctor. Integer eget mi vel ligula finibus rhoncus. Donec suscipit sem quis elit dapibus, in vestibulum dolor efficitur."
chunk5="Vestibulum aliquet tortor id quam lobortis, vel tempus nunc aliquet. Aenean sit amet interdum ipsum, non tempor arcu. Donec feugiat, arcu vitae egestas imperdiet, mauris leo ornare leo, et molestie purus mi gravida augue. Morbi et justo id justo laoreet commodo. Etiam vitae lorem sagittis dolor vestibulum mattis. Integer ac venenatis turpis. Nulla pellentesque leo posuere enim gravida rhoncus. Aliquam laoreet massa tortor, vitae dignissim odio fringilla nec. Duis vitae erat lacus. Fusce sed nibh sit amet nulla malesuada rutrum. Cras sit amet purus orci. Sed porttitor id tortor sed consequat. Curabitur ultricies mi a mi eleifend laoreet. Aliquam at condimentum est."
chunk6="Phasellus nec lacus odio. Maecenas odio urna, malesuada eu mattis sit amet, fermentum quis turpis. Nulla blandit congue mollis. Curabitur dictum tincidunt tempor. Donec auctor tincidunt cursus. Morbi a augue consequat, commodo est gravida, sagittis urna. Nunc convallis luctus purus, sit amet mattis ligula lacinia ac. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus mattis, dui vel porta viverra, lacus lorem vehicula erat, ut elementum nisi nunc vitae urna. Donec non erat lorem. Donec at dui eget est pellentesque semper vel ac urna. Vestibulum aliquet urna ut diam iaculis auctor. Nam luctus, nulla in aliquet faucibus, ligula leo finibus elit, vehicula tristique erat justo id ex."
chunk7="Donec et pharetra diam, et consectetur orci. Sed hendrerit purus nulla, eu facilisis massa euismod eget. Aliquam sed enim enim. Integer venenatis enim vitae libero pharetra efficitur. Sed dui ante, convallis vel rhoncus id, dignissim in orci. Nulla congue aliquam purus, sed pharetra ligula rutrum ut. Etiam sit amet nunc maximus, ultricies libero at, gravida eros. Morbi laoreet accumsan urna, in suscipit lorem interdum nec. Fusce efficitur tempus hendrerit. Duis ultrices velit quis porta tincidunt. Curabitur ac mi a turpis mattis commodo sed at odio. Suspendisse pulvinar tellus a turpis posuere viverra. Suspendisse vehicula felis vitae finibus ultrices."
chunk8="Duis nec condimentum neque, ut feugiat leo. Pellentesque ut dictum nibh, ac egestas nulla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Duis vestibulum enim et enim placerat gravida. Morbi non non."

# (echo -ne "$req\r\n$host\r\n$option\r\nTransfer-Encoding: chunked\r\n\r\n"; \
# echo -ne "4d\r\ndata=djhksdhjdshfkjshdfjkshdfjhskdhsjkdfhskjdhfkjsdfhjksdfhkjsdhfkjsdhfkjsdhf\r\n"; \
# echo -ne "0\r\n\r\n") | nc lala.42.fr 8080

(echo -ne "$req\r\n$host\r\n$option\r\nTransfer-Encoding: chunked\r\n\r\n"; \
echo -ne "5\r\ndata=\r\n"; \
echo -ne "2cb\r\n$chunk1\n\r\n"; \
echo -ne "190\r\n$chunk2\n\r\n"; \
echo -ne "32c\r\n$chunk3\n\r\n"; \
echo -ne "2c3\r\n$chunk4\n\r\n"; \
sleep 1;
echo -ne "2a1\r\n$chunk5\n\r\n"; \
echo -ne "2c1\r\n$chunk6\n\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
sleep 1;
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "296\r\n$chunk7\n0\r\n"; \
echo -ne "14f\r\n$chunk8\n\r\n"; \
echo -ne "0\r\n") | nc lala.42.fr 8080


# (echo -e "$req\r\n$host\r\nTransfer-Encoding: chunked\r\n\r\n"; \
# echo -e "6\r\nhello\r\n"; \
# echo -e "4\r\ntest\r\n"; \
# echo -e "0\r\n\r\n") | nc lala.42.fr 8080

# req="POST /randombis.php HTTP/1.1"
# host="Host: bsuc.42.fr"

# (echo -e "$req\r\n$host\r\nTransfer-Encoding: chunked\r\n\r\n"; \
# echo -e "5\r\nhello\r\n"; \
# echo -e "4\r\ntest\r\n"; \
# echo -e "0\r\n\r\n") | nc bsuc.42.fr 7700
