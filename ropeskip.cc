#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
#include <random>

bool ely = false;
bool splash = false;
int magicD = 0;
int rangeD = 0;
int accRange = 0;
int accMage = 0;
int maxMage = 0;
int maxRange = 0;
int count = 0;
random_device rd;
//mt19937 gen(rd());
mt19937 gen(time(NULL));
uniform_int_distribution<int> elyDist(1,100);

int getRandom(int from, int to) {
	uniform_int_distribution<int> distribution(from,to);
	return distribution(gen);
}

int getEly(){
	if (elyDist(gen) <= 70) return 4;
	return 3;
}

int doEly(int x){
	return 3*x/getEly();
}


int hit(int atk, int def, int max) {
	double atkRoll = getRandom(0,atk);
	double defRoll = getRandom(0,def);
	if ( atkRoll > defRoll ) {
		splash = false;
		if (ely) return doEly(getRandom(0,max));
		else return getRandom(0,max);
	} else {
		splash = true;
		return 0;
	}
	
}

//probably shoulda put +8 to stat here, but instead I do it manually when I input variables for it
//salve is for boosts like salve or void
int calcAD(int equip, int stat, double salve) {
	return (int)((equip+64)*stat*salve);
}

// cross for old kirby skip
int cross(int hp, int heal, bool sweet, bool redemption) {
	int cap = hp;
	int prayProt = 1;
	bool redempStrat = redemption;
	if (hp == 2 && !redemption) prayProt = 2;
	hp += heal;
	if (sweet) hp += getRandom(1,3);

	// first cross
	for (int i = 0; i < 6; i++) {
		hp -= min(cap,hit(accMage, magicD, maxMage)/prayProt);
		if ( hp < 10 && hp > 0 && redemption) {
			redemption = false;
			hp += 24; 
		}
		hp -= min(cap,hit(accRange, rangeD, maxRange));
		if ( hp < 10 && hp > 0 && redemption) {
			redemption = false;
			hp += 24;
		}

	}


	// check if dead or guaranteed survival
	if (hp < 1) return 1;
	cap = hp;
	if (!redempStrat) {
		hp++;
		if (hp < 21) return 0;
	} else if (hp < 19) return 0;

	// second cross
	for (int i = 0; i < 4; i++) {
		// mage hit
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 1 ) {
			if (!redempStrat) return 1;
			break;
		}
		if ( hp < 20 && !splash ) return 0;

		//range hit
		hp -= min(cap,hit(accRange, rangeD, maxRange));
		if ( hp < 1 ) {
			if (!redempStrat) return 1;
			break;
		}
		if ( hp < 20 ) return 0;
	}
	if ( redempStrat ) {
		if (hp < 0) hp = 0;
		hp += 19;
	}

	// remainder of second cross
	for (int i = 0; i < 2; i++) {
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 1 ) return 1;
		if ( hp < 20 && !splash ) return 0;

		//range hit
		hp -= min(cap,hit(accRange, rangeD, maxRange));
		if ( hp < 1 ) return 1;
		if ( hp < 20 ) return 0;
	}
	return 0;

}

// ignore this function, wasn't done as I switched to focusing on new method
/*
int doubleNeck(int hp, int heal, bool sweet) {
	int cap = hp;
	hp++;


	// first cross
	for (int i = 0; i < 6; i++) {
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 20 && !splash ) {
			hp += 30;
			break;
		}
		hp -= min(cap,hit(accRange, rangeD, maxRange)/2);
		if ( hp < 20 && hp > 0 ) {
			hp += 30;
			break;
		}
	}
	if ( hp <= 0 ) {
		cout <<"Something went wrong" << endl;
		return 1;
	}

	hp += heal;
	if (sweet) hp += getRandom(1,3);

	cap = hp;
	hp += getRandom(1,3);

	// second cross
	for (int i = 0; i < 8; i++) {
		// mage hit
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 1 ) return 1;
		if ( hp < 20 && !splash ) return 0;

		//range hit
		hp -= min(cap,hit(accRange, rangeD, maxRange)/2);
		if ( hp < 1 ) return 1;
		if ( hp < 20 ) return 0;
	}
	
	// remainder of second cross
	for (int i = 0; i < 2; i++) {
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 1 ) return 1;
		if ( hp < 20 && !splash ) return 0;

		//range hit
		hp -= min(cap,hit(accRange, rangeD, maxRange));
		if ( hp < 1 ) return 1;
		if ( hp < 20 ) return 0;
	}
	return 0;

}
*/


int singleNeck(int hp, bool sweetTwo) {
	int cap = hp;
	int tempSweet = 0;
	if (hp == 3) hp += 19;
	if (sweetTwo && hp == 2) hp += getRandom(1,3);
	hp += 24+1;


	// first cross
	for (int i = 0; i < 6; i++) {
		if (i == 4) cap++;
		hp -= min(cap,hit(accMage, magicD, maxMage));
		hp -= min(cap,hit(accRange, rangeD, maxRange));
	}
	if (hp < 1) return 1;

	cap = hp;
	tempSweet = getRandom(1,3);
	hp += tempSweet;


	// second cross
	for (int i = 0; i < 2; i++) {
		// mage hit
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 1 ) return 1;
		if ( hp < 20 && !splash ) return 0;

		//range hit
		hp -= min(cap,hit(accRange, rangeD, maxRange));
		if ( hp < 1 ) return 1;
		if ( hp < 20 ) return 0;
	}

	cap += tempSweet;

	// second cross
	for (int i = 0; i < 6; i++) {
		// mage hit
		hp -= min(cap,hit(accMage, magicD, maxMage));
		if ( hp < 1 ) return 1;
		if ( hp < 20 && !splash ) return 0;

		//range hit
		hp -= min(cap,hit(accRange, rangeD, maxRange));
		if ( hp < 1 ) return 1;
		if ( hp < 20 ) return 0;
	}

	return 0;

}

int main() {
	int elyBonus = 0;
	ely = true;
	if (ely) elyBonus = 5;
	srand (time(NULL));
	int armaM = 156; int armaR = 187; //+5 for ely 
	int nakedM = 9; int nakedR = 14;
	int inquiM = 15; int inquiR = 83;
	int bandosM = 83; int bandosR = 285; //+5 for ely
	//int mDef = inquiM;
	//int rDef = inquiR;
	int mDef = armaM;
	int rDef = armaR + elyBonus;
	//int mDef = nakedM;
	//int rDef = nakedR;
	//int mDef = bandosM;
	//int rDef = bandosR + elyBonus;
	int highDefM = calcAD(mDef,0.7*150 + 0.3*161,1);
	int highDefR = calcAD(rDef,153+8,1); // bandos chestplate is +133 range -6 magic
	int lowDefM = calcAD(mDef,120*0.7 + 131*0.3,1);//-6 for barrows, 136 for no pray, 2 for ely
	int lowDefR = calcAD(rDef,123+8,1); //-47 for inqui body, 100 for no pray, 57 for ely, 22 i legs
	int rigourDefM = calcAD(mDef,0.7*120 + 0.3*161,1);
	int rigourDefR = calcAD(rDef,153+8,1);
	cout << "Magic defence is " << lowDefM << ", Range defence is " << lowDefR << "." << endl;
	cout << "Magic defence is " << highDefM << ", Range defence is " << highDefR << "." << endl;
	cout << "Magic defence is " << rigourDefM << ", Range defence is " << rigourDefR << "." << endl;
	magicD = calcAD(mDef,120*0.7 + 131*0.3,1);
	rangeD = calcAD(rDef,123+8,1);
	accMage = calcAD(120,315+9,1);
	accRange = calcAD(120,315+9,1);
	maxMage = 32;  //pray does half exactly
	maxRange = 68; //pray does half exatly
	int twohpdeath = 0;
	int threehpdeath = 0;
	int twohpdeathsweet = 0;
	int twohpredemp = 0;
	int threehpredemp = 0;
	int fourhpredemp = 0;
	int fourhpredempsweet = 0;
	int neckDeaths = 0;
	int neckDeathsOne = 0;
	int neckDeathsTwo = 0;
	int neckDeathsThree = 0;
	int neckDeathsTwoNoSweet = 0;
	int realHP = 0;
	
	// cross(hp, heal, sweet, redemption);
	// singleNeck(hp, sweet);
	try {
		int loops = 1000000;

		//magicD = highDefM;
		//rangeD = highDefR;
		//magicD = rigourDefM;
		//rangeD = rigourDefR;
		magicD = lowDefM;
		rangeD = lowDefR;
		cout << "Magic defence is " << magicD << ", Range defence is " << rangeD << "." << endl;
		for (int x = 0; x < loops; x++) {
			
			//check total death rate taking into account chance of being on 1 vs 2 vs 3 hp
			/*
			realHP = getRandom(10,33);
			while (true) {
				if (realHP > 12 && realHP < 16) realHP += 19;
				if (realHP > 31 && realHP < 35) {
					realHP += 19;
					realHP -= 50;
					break;
				}
				realHP += getRandom(1,3);
			}
			if (realHP == 2) neckDeaths += singleNeck(realHP,true); //set to false if not sweeting for 2hp
			else neckDeaths += singleNeck(realHP,false);
			*/

			neckDeathsOne += singleNeck(1,false);
			neckDeathsTwo += singleNeck(2,true);
			neckDeathsThree += singleNeck(3,false);
			neckDeathsTwoNoSweet += singleNeck(2,false);
		}


		// 2 hp normal method for old kirby skip
		/*
		magicD = highDefM;
		rangeD = highDefR;
		for (int x = 0; x < loops; x++) {
			twohpdeath += cross(2, 19, false, false);
			twohpdeathsweet += cross(2, 19, true, false);
		}
		*/

		// 3hp, 3hp redemp, and 4hp redemp for old kirby skip
		/*
		magicD = lowDefM;
		rangeD = lowDefR;
		for (int x = 0; x < loops; x++) {
			threehpdeath += cross(3, 19+23, false, false);
			twohpredemp += cross(2, 0, true, true);
			threehpredemp += cross(3, 19, false, true);
			fourhpredemp += cross(4, 19, false, true);
			fourhpredempsweet += cross(4, 19, true, true);
		}
		*/

		cout << neckDeathsOne << " 1 hp deaths. " << (double)(neckDeathsOne)/10000 << "%" << endl;
		cout << neckDeathsTwo << " 2 hp deaths. " << (double)(neckDeathsTwo)/10000 << "%" << endl;
		cout << neckDeathsThree << " 3 hp deaths. " << (double)(neckDeathsThree)/10000 << "%" << endl;
		cout << neckDeathsTwoNoSweet << " 2 hp deaths ( no sweet). " << (double)(neckDeathsTwoNoSweet)/10000 << "%" << endl;
		//cout << neckDeaths << " overall deaths. " << (double)(neckDeaths)/10000 << "%" << endl;
		//cout << twohpdeath << " 2hp deaths. " << (double)(twohpdeath)/10000 << "%" << endl;
		//cout << threehpdeath << " 3hp deaths. " << (double)(threehpdeath)/10000 << "%" << endl;
		//cout << twohpdeathsweet << " 2hp sweet deaths. " << (double)(twohpdeathsweet)/10000 << "%" << endl;
		//cout << twohpredemp << " 2hp redemption deaths. " << (double)(twohpredemp)/10000 << "%" << endl;
		//cout << threehpredemp << " 3hp redemption deaths. " << (double)(threehpredemp)/10000 << "%" << endl;
		//cout << fourhpredemp << " 4hp redemption deaths. " << (double)(fourhpredemp)/10000 << "%" << endl;
		//cout << fourhpredempsweet << " 4 hp redemption sweet deaths. " << (double)(fourhpredempsweet)/10000 << "%" << endl;
		//cout << count << " is count. " << (double)(count)/10000 << "%" << endl;
	}
	catch ( ... ) {
		cout << "Ending Program" << endl;
	}
	cout << "done program lol" << endl;
}