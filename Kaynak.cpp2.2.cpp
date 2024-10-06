#include <iostream>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <string>

using namespace std;

// Hasta veri yapısı
struct Hasta {
    string tc;
    string adSoyad;
};

// Randevu veri yapısı
struct Randevu {
    string hastane;
    string saat;
    string gun;
    Hasta hasta;
};

class MHRS {
private:
    unordered_map<string, stack<Randevu>> randevular; // Hastane ismi ile randevuları ilişkilendir
    unordered_map<string, unordered_map<string, bool>> randevuDurumu; // Gun ve saat ile randevu durumunu tut
    stack<Randevu> iptalEdilenRandevular; // İptal edilen randevular

public:
    // Randevu al
    void randevuAl(string hastane, string saat, string gun, string tc, string adSoyad) {
        // Belirtilen saat ve günde randevu var mı kontrol et
        if (randevuDurumu[gun][saat]) {
            cout << "Belirtilen saat ve günde zaten randevu alınmıştır." << endl;
            return;
        }

        Randevu randevu = { hastane, saat, gun, {tc, adSoyad} };
        randevular[hastane].push(randevu);
        randevuDurumu[gun][saat] = true;

        // Hatırlatıcı mesajı gönder
        hatirlaticiMesajGonder(randevu);

        cout << "Randevu alındı: " << hastane << " - " << gun << " - " << saat << " - " << adSoyad << endl;
    }

    // Randevu iptal et
    void randevuIptal(string hastane, string saat, string gun, string tc) {
        if (randevular[hastane].empty()) {
            cout << "Randevu bulunamadı." << endl;
            return;
        }

        stack<Randevu>& randevuStack = randevular[hastane];
        stack<Randevu> tempStack;

        while (!randevuStack.empty()) {
            Randevu randevu = randevuStack.top();
            randevuStack.pop();
            if (randevu.gun == gun && randevu.saat == saat && randevu.hasta.tc == tc) {
                cout << "Randevu iptal edildi: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.saat << " - " << randevu.hasta.adSoyad << endl;
                randevuDurumu[gun][saat] = false;
                iptalEdilenRandevular.push(randevu); // İptal edilen randevuyu kaydet

                // İptal edilen randevuyu dosyaya yaz
                ofstream dosya("randevubilgilendirme.txt", ios::app);
                dosya << "Randevunuz iptal olmuştur: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.hasta.adSoyad << endl;
                dosya.close();
                break;
            }
            else {
                tempStack.push(randevu);
            }
        }

        while (!tempStack.empty()) {
            randevuStack.push(tempStack.top());
            tempStack.pop();
        }
    }

    // Randevuyu geri al
    void randevuyuGeriAl() {
        if (iptalEdilenRandevular.empty()) {
            cout << "Geri alınacak iptal edilen randevu bulunmamaktadır." << endl;
            return;
        }

        Randevu randevu = iptalEdilenRandevular.top();
        iptalEdilenRandevular.pop();

        if (randevuDurumu[randevu.gun][randevu.saat]) {
            cout << "Randevunun geri alınması başarısız oldu. Aynı saat ve günde başka bir randevu mevcut." << endl;
        }
        else {
            randevular[randevu.hastane].push(randevu);
            randevuDurumu[randevu.gun][randevu.saat] = true;
            cout << "Randevu geri alındı: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.saat << " - " << randevu.hasta.adSoyad << endl;

            // Geri alınan randevuyu dosyaya yaz
            ofstream dosya("randevubilgilendirme.txt", ios::app);
            dosya << randevu.hasta.adSoyad << "@gmail.com\nPOSTA KUTUSU \n 1 YENİ GÖNDERİ " << endl;
            dosya << "Randevunuz geri alınmıştır: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.hasta.adSoyad << endl;
            dosya.close();
        }
    }

private:
    // Hatırlatıcı mesaj gönderme işlevi
    void hatirlaticiMesajGonder(Randevu randevu) {
        cout << "Hatırlatıcı: Randevunuz var!" << endl;
        cout << "Hastane: " << randevu.hastane << endl;
        cout << "Tarih: " << randevu.gun << " Saat: " << randevu.saat << endl;
        cout << "Hasta: " << randevu.hasta.adSoyad << endl;

        // Hatırlatıcı mesajı dosyaya yaz
        ofstream dosya("randevubilgilendirme.txt", ios::app);
        dosya << randevu.hasta.adSoyad << "@gmail.com\nPOSTA KUTUSU \n 1 YENİ GÖNDERİ " << endl;
        dosya << "Hatırlatıcı: Randevunuz var!" << endl;
        dosya << "Hastane: " << randevu.hastane << endl;
        dosya << "Tarih: " << randevu.gun << " Saat: " << randevu.saat << endl;
        dosya << "Hasta: " << randevu.hasta.adSoyad << endl;
        dosya << "------------------------------------" << endl;
        dosya.close();
    }
};

int main() {
    setlocale(LC_ALL, "Turkısh");
    MHRS mhrs;

    while (true) {
        cout << "1. Randevu Al" << endl;
        cout << "2. Randevu Iptal Et" << endl;
        cout << "3. Randevuyu Geri Al" << endl;
        cout << "4. Cikis" << endl;
        cout << "Seciminizi yapiniz: ";
        int secim;
        cin >> secim;

        if (secim == 1) {
            string hastane, saat, gun, tc, adSoyad;
            cout << "Hastane seciniz: ";
            cin >> hastane;
            cout << "Saat seciniz: ";
            cin >> saat;
            cout << "Gun seciniz: ";
            cin >> gun;
            cout << "TC giriniz: ";
            cin >> tc;
            cout << "Ad soyad giriniz: ";
            cin.ignore(); // Bufferı temizle
            getline(cin, adSoyad);
            mhrs.randevuAl(hastane, saat, gun, tc, adSoyad);
        }
        else if (secim == 2) {
            string hastane, saat, gun, tc;
            cout << "Hastane seciniz: ";
            cin >> hastane;
            cout << "Saat seciniz: ";
            cin >> saat;
            cout << "Gun seciniz: ";
            cin >> gun;
            cout << "TC giriniz: ";
            cin >> tc;
            mhrs.randevuIptal(hastane, saat, gun, tc);
        }
        else if (secim == 3) {
            mhrs.randevuyuGeriAl();
        }
        else if (secim == 4) {
            break;
        }
        else {
            cout << "Gecersiz secim!" << endl;
        }
    }

    return 0;
}
