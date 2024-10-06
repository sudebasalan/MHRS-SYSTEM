#include <iostream>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <string>

using namespace std;

// Hasta veri yap�s�
struct Hasta {
    string tc;
    string adSoyad;
};

// Randevu veri yap�s�
struct Randevu {
    string hastane;
    string saat;
    string gun;
    Hasta hasta;
};

class MHRS {
private:
    unordered_map<string, stack<Randevu>> randevular; // Hastane ismi ile randevular� ili�kilendir
    unordered_map<string, unordered_map<string, bool>> randevuDurumu; // Gun ve saat ile randevu durumunu tut
    stack<Randevu> iptalEdilenRandevular; // �ptal edilen randevular

public:
    // Randevu al
    void randevuAl(string hastane, string saat, string gun, string tc, string adSoyad) {
        // Belirtilen saat ve g�nde randevu var m� kontrol et
        if (randevuDurumu[gun][saat]) {
            cout << "Belirtilen saat ve g�nde zaten randevu al�nm��t�r." << endl;
            return;
        }

        Randevu randevu = { hastane, saat, gun, {tc, adSoyad} };
        randevular[hastane].push(randevu);
        randevuDurumu[gun][saat] = true;

        // Hat�rlat�c� mesaj� g�nder
        hatirlaticiMesajGonder(randevu);

        cout << "Randevu al�nd�: " << hastane << " - " << gun << " - " << saat << " - " << adSoyad << endl;
    }

    // Randevu iptal et
    void randevuIptal(string hastane, string saat, string gun, string tc) {
        if (randevular[hastane].empty()) {
            cout << "Randevu bulunamad�." << endl;
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
                iptalEdilenRandevular.push(randevu); // �ptal edilen randevuyu kaydet

                // �ptal edilen randevuyu dosyaya yaz
                ofstream dosya("randevubilgilendirme.txt", ios::app);
                dosya << "Randevunuz iptal olmu�tur: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.hasta.adSoyad << endl;
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
            cout << "Geri al�nacak iptal edilen randevu bulunmamaktad�r." << endl;
            return;
        }

        Randevu randevu = iptalEdilenRandevular.top();
        iptalEdilenRandevular.pop();

        if (randevuDurumu[randevu.gun][randevu.saat]) {
            cout << "Randevunun geri al�nmas� ba�ar�s�z oldu. Ayn� saat ve g�nde ba�ka bir randevu mevcut." << endl;
        }
        else {
            randevular[randevu.hastane].push(randevu);
            randevuDurumu[randevu.gun][randevu.saat] = true;
            cout << "Randevu geri al�nd�: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.saat << " - " << randevu.hasta.adSoyad << endl;

            // Geri al�nan randevuyu dosyaya yaz
            ofstream dosya("randevubilgilendirme.txt", ios::app);
            dosya << randevu.hasta.adSoyad << "@gmail.com\nPOSTA KUTUSU \n 1 YEN� G�NDER� " << endl;
            dosya << "Randevunuz geri al�nm��t�r: " << randevu.hastane << " - " << randevu.gun << " - " << randevu.hasta.adSoyad << endl;
            dosya.close();
        }
    }

private:
    // Hat�rlat�c� mesaj g�nderme i�levi
    void hatirlaticiMesajGonder(Randevu randevu) {
        cout << "Hat�rlat�c�: Randevunuz var!" << endl;
        cout << "Hastane: " << randevu.hastane << endl;
        cout << "Tarih: " << randevu.gun << " Saat: " << randevu.saat << endl;
        cout << "Hasta: " << randevu.hasta.adSoyad << endl;

        // Hat�rlat�c� mesaj� dosyaya yaz
        ofstream dosya("randevubilgilendirme.txt", ios::app);
        dosya << randevu.hasta.adSoyad << "@gmail.com\nPOSTA KUTUSU \n 1 YEN� G�NDER� " << endl;
        dosya << "Hat�rlat�c�: Randevunuz var!" << endl;
        dosya << "Hastane: " << randevu.hastane << endl;
        dosya << "Tarih: " << randevu.gun << " Saat: " << randevu.saat << endl;
        dosya << "Hasta: " << randevu.hasta.adSoyad << endl;
        dosya << "------------------------------------" << endl;
        dosya.close();
    }
};

int main() {
    setlocale(LC_ALL, "Turk�sh");
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
            cin.ignore(); // Buffer� temizle
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
