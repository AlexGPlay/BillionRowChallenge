#include <stdio.h>
#include <stdlib.h>

#define MEASUREMENTS 1000000000
#define FILENAME "measurements.txt"

#define ARRAY_SIZE 413

int main(){
  char* stations[ARRAY_SIZE] = {
    "Abha","Abidjan","Abéché","Accra","Addis Ababa","Adelaide","Aden","Ahvaz","Albuquerque","Alexandra","Alexandria","Algiers","Alice Springs","Almaty","Amsterdam","Anadyr","Anchorage","Andorra la Vella","Ankara","Antananarivo","Antsiranana","Arkhangelsk","Ashgabat","Asmara","Assab","Astana","Athens","Atlanta","Auckland","Austin","Baghdad","Baguio","Baku","Baltimore","Bamako","Bangkok","Bangui","Banjul","Barcelona","Bata","Batumi","Beijing","Beirut","Belgrade","Belize City","Benghazi","Bergen","Berlin","Bilbao","Birao","Bishkek","Bissau","Blantyre","Bloemfontein","Boise","Bordeaux","Bosaso","Boston","Bouaké","Bratislava","Brazzaville","Bridgetown","Brisbane","Brussels","Bucharest","Budapest","Bujumbura","Bulawayo","Burnie","Busan","Cabo San Lucas","Cairns","Cairo","Calgary","Canberra","Cape Town","Changsha","Charlotte","Chiang Mai","Chicago","Chihuahua","Chișinău","Chittagong","Chongqing","Christchurch","City of San Marino","Colombo","Columbus","Conakry","Copenhagen","Cotonou","Cracow","Da Lat","Da Nang","Dakar","Dallas","Damascus","Dampier","Dar es Salaam","Darwin","Denpasar","Denver","Detroit","Dhaka","Dikson","Dili","Djibouti","Dodoma","Dolisie","Douala","Dubai","Dublin","Dunedin","Durban","Dushanbe","Edinburgh","Edmonton","El Paso","Entebbe","Erbil","Erzurum","Fairbanks","Fianarantsoa","Flores, Petén","Frankfurt","Fresno","Fukuoka","Gabès","Gaborone","Gagnoa","Gangtok","Garissa","Garoua","George Town","Ghanzi","Gjoa Haven","Guadalajara","Guangzhou","Guatemala City","Halifax","Hamburg","Hamilton","Hanga Roa","Hanoi","Harare","Harbin","Hargeisa","Hat Yai","Havana","Helsinki","Heraklion","Hiroshima","Ho Chi Minh City","Hobart","Hong Kong","Honiara","Honolulu","Houston","Ifrane","Indianapolis","Iqaluit","Irkutsk","Istanbul","İzmir","Jacksonville","Jakarta","Jayapura","Jerusalem","Johannesburg","Jos","Juba","Kabul","Kampala","Kandi","Kankan","Kano","Kansas City","Karachi","Karonga","Kathmandu","Khartoum","Kingston","Kinshasa","Kolkata","Kuala Lumpur","Kumasi","Kunming","Kuopio","Kuwait City","Kyiv","Kyoto","La Ceiba","La Paz","Lagos","Lahore","Lake Havasu City","Lake Tekapo","Las Palmas de Gran Canaria","Las Vegas","Launceston","Lhasa","Libreville","Lisbon","Livingstone","Ljubljana","Lodwar","Lomé","London","Los Angeles","Louisville","Luanda","Lubumbashi","Lusaka","Luxembourg City","Lviv","Lyon","Madrid","Mahajanga","Makassar","Makurdi","Malabo","Malé","Managua","Manama","Mandalay","Mango","Manila","Maputo","Marrakesh","Marseille","Maun","Medan","Mek'ele","Melbourne","Memphis","Mexicali","Mexico City","Miami","Milan","Milwaukee","Minneapolis","Minsk","Mogadishu","Mombasa","Monaco","Moncton","Monterrey","Montreal","Moscow","Mumbai","Murmansk","Muscat","Mzuzu","N'Djamena","Naha","Nairobi","Nakhon Ratchasima","Napier","Napoli","Nashville","Nassau","Ndola","New Delhi","New Orleans","New York City","Ngaoundéré","Niamey","Nicosia","Niigata","Nouadhibou","Nouakchott","Novosibirsk","Nuuk","Odesa","Odienné","Oklahoma City","Omaha","Oranjestad","Oslo","Ottawa","Ouagadougou","Ouahigouya","Ouarzazate","Oulu","Palembang","Palermo","Palm Springs","Palmerston North","Panama City","Parakou","Paris","Perth","Petropavlovsk-Kamchatsky","Philadelphia","Phnom Penh","Phoenix","Pittsburgh","Podgorica","Pointe-Noire","Pontianak","Port Moresby","Port Sudan","Port Vila","Port-Gentil","Portland (OR)","Porto","Prague","Praia","Pretoria","Pyongyang","Rabat","Rangpur","Reggane","Reykjavík","Riga","Riyadh","Rome","Roseau","Rostov-on-Don","Sacramento","Saint Petersburg","Saint-Pierre","Salt Lake City","San Antonio","San Diego","San Francisco","San Jose","San José","San Juan","San Salvador","Sana'a","Santo Domingo","Sapporo","Sarajevo","Saskatoon","Seattle","Ségou","Seoul","Seville","Shanghai","Singapore","Skopje","Sochi","Sofia","Sokoto","Split","St. John's","St. Louis","Stockholm","Surabaya","Suva","Suwałki","Sydney","Tabora","Tabriz","Taipei","Tallinn","Tamale","Tamanrasset","Tampa","Tashkent","Tauranga","Tbilisi","Tegucigalpa","Tehran","Tel Aviv","Thessaloniki","Thiès","Tijuana","Timbuktu","Tirana","Toamasina","Tokyo","Toliara","Toluca","Toronto","Tripoli","Tromsø","Tucson","Tunis","Ulaanbaatar","Upington","Ürümqi","Vaduz","Valencia","Valletta","Vancouver","Veracruz","Vienna","Vientiane","Villahermosa","Vilnius","Virginia Beach","Vladivostok","Warsaw","Washington, D.C.","Wau","Wellington","Whitehorse","Wichita","Willemstad","Winnipeg","Wrocław","Xi'an","Yakutsk","Yangon","Yaoundé","Yellowknife","Yerevan","Yinchuan","Zagreb","Zanzibar City","Zürich"
  };

  float temperatures[ARRAY_SIZE] = {
    18,26,29.4,26.4,16,17.3,29.1,25.4,14,11,20,18.2,21,10,10.2,-6.9,2.8,9.8,12,17.9,25.2,1.3,17.1,15.6,30.5,3.5,19.2,17,15.2,20.7,22.77,19.5,15.1,13.1,27.8,28.6,26,26,18.2,25.1,14,12.9,20.9,12.5,26.7,19.9,7.7,10.3,14.7,26.5,11.3,27,22.2,15.6,11.4,14.2,30,10.9,26,10.5,25,27,21.4,10.5,10.8,11.3,23.8,18.9,13.1,15,23.9,25,21.4,4.4,13.1,16.2,17.4,16.1,25.8,9.8,18.6,10.2,25.9,18.6,12.2,11.8,27.4,11.7,26.4,9.1,27.2,9.3,17.9,25.8,24,19,17,26.4,25.8,27.6,23.7,10.4,10,25.9,-11.1,26.6,29.9,22.7,24,26.7,26.9,9.8,11.1,20.6,14.7,9.3,4.2,18.1,21,19.5,5.1,-2.3,17.9,26.4,10.6,17.9,17,19.5,21,26,15.2,29.3,28.3,27.9,21.4,-14.4,20.9,22.4,20.4,7.5,9.7,13.8,20.5,23.6,18.4,5,21.7,27,25.2,5.9,18.9,16.3,27.4,12.7,23.3,26.5,25.4,20.8,11.4,11.8,-9.3,1,13.9,17.9,20.3,26.7,27,18.3,15.5,22.8,27.8,12.1,20,27.7,26.5,26.4,12.5,26,24.4,18.3,29.9,27.4,25.3,26.7,27.3,26,15.7,3.4,25.7,8.4,15.8,26.2,23.7,26.8,24.3,23.7,8.7,21.2,20.3,13.1,7.6,25.9,17.5,21.8,10.9,29.3,26.9,11.3,18.6,13.9,25.8,20.8,19.9,9.3,7.8,12.5,15,26.3,26.7,26,26.3,28,27.3,26.5,28,28.1,28.4,22.8,19.6,15.8,22.4,26.5,22.7,15.1,17.2,23.1,17.5,24.9,13,8.9,7.8,6.7,27.1,26.3,16.4,6.1,22.3,6.8,5.8,27.1,0.6,28,17.7,28.3,23.1,17.8,27.3,14.6,15.9,15.4,24.6,20.3,25,20.7,12.9,22,29.3,19.7,13.9,21.3,25.7,1.7,-1.4,10.7,26,15.9,10.6,28.1,5.7,6.6,28.3,28.6,18.9,2.7,27.3,18.5,24.5,13.2,28,26.8,12.3,18.7,1.9,13.2,28.3,23.9,10.8,15.3,26.1,27.7,26.9,28.4,24.3,26,12.4,15.7,8.4,24.4,18.2,10.8,17.2,24.4,28.3,4.3,6.2,26,15.2,26.2,9.9,16.3,5.8,5.7,11.6,20.8,17.8,14.6,16.4,22.6,27.2,23.1,20,25.9,8.9,10.1,3.3,11.3,28,12.5,19.2,16.7,27,12.4,14.2,10.6,28,16.1,5,13.9,6.6,27.1,25.6,7.2,17.7,23,12.6,23,6.4,27.9,21.7,22.9,14.8,14.8,12.9,21.7,17,20,16,24,17.8,28,15.2,23.4,15.4,24.1,12.4,9.4,20,2.9,20.9,18.4,-0.4,20.4,7.4,10.1,18.3,18.8,10.4,25.4,10.4,25.9,27.1,6,15.8,4.9,8.5,14.6,27.8,12.9,-0.1,13.9,28,3,9.6,14.1,-8.8,27.5,23.8,-4.3,12.4,9,10.7,26,9.3
  };

  FILE* file = fopen(FILENAME, "w");
  if(file == NULL){
    printf("Error opening file\n");
    return 1;
  }

  for(int i = 0; i < MEASUREMENTS; i++){
    int random_index = rand() % ARRAY_SIZE;

    char* station = stations[random_index];
    float temperature = temperatures[random_index];

    float measurement = (temperature + (rand() % 21 - 10) / 10.0);

    fprintf(file, "%s;%.1f\n", station, measurement);

    if(i % 1000000 == 0){
      printf("Progress: %d/%d\n", i, MEASUREMENTS);
    }
  }

  fclose(file);

  return 0;
}