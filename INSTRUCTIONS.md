Connexions WIFI :
- La carte vérifie en temps réel la présence d'un réseau connu :
  nmcli d
  nmcli r wifi on
  nmcli d wifi list > ./tmp/connexions.txt
  - Si la carte détecte ce réseau : Se connecter à ce réseau en ligne de commande, ce qui désactive le router de la carte.
      ls /etc/NetworkManager/system-connections > ./history/connexions.txt # Liste les SSID déjà utilisés avec leur conf à l'interieur
      sudo nmcli d wifi connect SSID password PSK # Connecte à un réseau en ligne de commande
  - Sinon si la carte n'est pas connectée à internet : Redémarrer le service de router de la carte
      sudo systemctl restart hostapd