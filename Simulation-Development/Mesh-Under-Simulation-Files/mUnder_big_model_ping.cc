//Author: Kevin Mc Gee
/*
// Network topology: Large(See corresponding portfolio - Appendix C for design details)
//
//
//===================
// WSN (802.15.4)
*/

/*Test: 
 * n0 = UDP SRC
 * n1 = ping SRC(1200B @ 0.5Hz)
 * n2 = root/relay node
 * n3 = DST
 * */

//Based on the examples wsn-ping6.cc, ping6.cc, example-ping-lr-wpan.cc and
//udp-echo.cc

#include "ns3/core-module.h"
#include "ns3/lr-wpan-module.h" 
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/applications-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"
#include "ns3/internet-stack-helper.h"
#include <ns3/ipv6-address-helper.h>
#include <ns3/sixlowpan-helper.h>
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include <ns3/spectrum-value.h>
#include <ns3/lr-wpan-spectrum-value-helper.h>
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv6-flow-classifier.h"
#include "ns3/ping6-helper.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("WSN");



int main(){
// Enable calculation of FCS in the trailers. Only necessary when interacting with real devices or wireshark.(Tommaso Pecorella)
GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

LogComponentEnable ("WSN", LOG_LEVEL_INFO);	
LogComponentEnable ("Socket", LOG_LEVEL_INFO);



//Each device must be attached to same channel - 
Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>(); 
Ptr<FriisPropagationLossModel> propModel = CreateObject<FriisPropagationLossModel>();
Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel>();
channel->AddPropagationLossModel(propModel);
channel->SetPropagationDelayModel(delayModel);

LrWpanHelper lrWpanHelper;
NetDeviceContainer lrwpanDevices;

Ptr<Node> allNodes[85];
Ptr<LrWpanNetDevice> dev[85];
Ptr<ConstantPositionMobilityModel> sender[85];
NodeContainer nodes;

//deprecated stuff for channel tuning
double txPower = 3;
uint32_t channelNumber = 11;
//configure power of transmission
LrWpanSpectrumValueHelper svh;
Ptr<SpectrumValue> psd = svh.CreateTxPowerSpectralDensity (txPower, channelNumber);

int iterator = 0;
for(iterator = 0; iterator < 85; iterator++){
	allNodes[iterator] = CreateObject<Node>(); //initialise(define)
	nodes.Add(allNodes[iterator]);//add to container

	dev[iterator] = CreateObject<LrWpanNetDevice>();
	lrwpanDevices.Add(dev[iterator]);

	//TODO: Set address

	//
	dev[iterator]->SetChannel(channel);
	dev[iterator]->GetPhy()->SetTxPowerSpectralDensity (psd);
	//
	allNodes[iterator]->AddDevice(dev[iterator]);

	sender[iterator] = CreateObject<ConstantPositionMobilityModel>();
}
//TODO: Position stuff + MAC addresses
/*
	KMG: The following work was manually written as looping over this difficult operation resulted in difficult run-time errors
*/
/*A*/
sender[0]->SetPosition(Vector(76.5,0,0));
dev[0]->GetPhy()->SetMobility(sender[0]);
dev[0]->SetAddress(Mac16Address("00:01"));


/*G*/
sender[1]->SetPosition(Vector(44.2,15.3,0));
dev[1]->GetPhy()->SetMobility(sender[1]);
dev[1]->SetAddress(Mac16Address("00:02"));
/*G1*/
sender[2]->SetPosition(Vector(37.4,6.5,0));
dev[2]->GetPhy()->SetMobility(sender[2]);
dev[2]->SetAddress(Mac16Address("00:03"));
/*G2*/
sender[3]->SetPosition(Vector(51.7,6.5,0));
dev[3]->GetPhy()->SetMobility(sender[3]);
dev[3]->SetAddress(Mac16Address("00:04"));
/*G3*/
sender[4]->SetPosition(Vector(39.1,13.6,0));
dev[4]->GetPhy()->SetMobility(sender[4]);
dev[4]->SetAddress(Mac16Address("00:05"));
/*G4*/
sender[5]->SetPosition(Vector(38.1,19.4,0));
dev[5]->GetPhy()->SetMobility(sender[5]);
dev[5]->SetAddress(Mac16Address("00:06"));
/*G5*/
sender[6]->SetPosition(Vector(42.2,17.3,0));
dev[6]->GetPhy()->SetMobility(sender[6]);
dev[6]->SetAddress(Mac16Address("00:07"));
/*G6*/
sender[7]->SetPosition(Vector(46.9,17.3,0));
dev[7]->GetPhy()->SetMobility(sender[7]);
dev[7]->SetAddress(Mac16Address("00:08"));
/*G7*/
sender[8]->SetPosition(Vector(50.7,19.4,0));
dev[8]->GetPhy()->SetMobility(sender[8]);
dev[8]->SetAddress(Mac16Address("00:09"));
/*G8*/
sender[9]->SetPosition(Vector(55.4,18.7,0));
dev[9]->GetPhy()->SetMobility(sender[9]);
dev[9]->SetAddress(Mac16Address("00:0a"));
/*G9*/
sender[10]->SetPosition(Vector(50.7,13.6,0));
dev[10]->GetPhy()->SetMobility(sender[10]);
dev[10]->SetAddress(Mac16Address("00:0b"));
/*G10*/
sender[11]->SetPosition(Vector(53.4,13.6,0));
dev[11]->GetPhy()->SetMobility(sender[11]);
dev[11]->SetAddress(Mac16Address("00:0c"));
/*G11(C1)*/
sender[12]->SetPosition(Vector(36.4,13.6,0));
dev[12]->GetPhy()->SetMobility(sender[12]);
dev[12]->SetAddress(Mac16Address("00:0d"));
/*G12(D1)*/
sender[13]->SetPosition(Vector(33.7,18.4,0));
dev[13]->GetPhy()->SetMobility(sender[13]);
dev[13]->SetAddress(Mac16Address("00:0e"));

/*I*/
sender[14]->SetPosition(Vector(73.1,15.3,0));
dev[14]->GetPhy()->SetMobility(sender[14]);
dev[14]->SetAddress(Mac16Address("00:0f"));
/*I1*/
sender[15]->SetPosition(Vector(70,18.7,0));
dev[15]->GetPhy()->SetMobility(sender[15]);
dev[15]->SetAddress(Mac16Address("00:10"));
/*I2*/
sender[16]->SetPosition(Vector(65.6,19.4,0));
dev[16]->GetPhy()->SetMobility(sender[16]);
dev[16]->SetAddress(Mac16Address("00:11"));
/*I3*/
sender[17]->SetPosition(Vector(62.6,18.4,0));
dev[17]->GetPhy()->SetMobility(sender[17]);
dev[17]->SetAddress(Mac16Address("00:12"));
/*I4*/
sender[18]->SetPosition(Vector(65.6,13.6,0));
dev[18]->GetPhy()->SetMobility(sender[18]);
dev[18]->SetAddress(Mac16Address("00:13"));
/*I5*/
sender[19]->SetPosition(Vector(68,13.6,0));
dev[19]->GetPhy()->SetMobility(sender[19]);
dev[19]->SetAddress(Mac16Address("00:14"));
/*I6*/
sender[20]->SetPosition(Vector(66.3,6.5,0));
dev[20]->GetPhy()->SetMobility(sender[20]);
dev[20]->SetAddress(Mac16Address("00:15"));
/*I7*/
sender[21]->SetPosition(Vector(80.9,6.5,0));
dev[21]->GetPhy()->SetMobility(sender[21]);
dev[21]->SetAddress(Mac16Address("00:16"));
/*I8*/
sender[22]->SetPosition(Vector(79.6,13.6,0));
dev[22]->GetPhy()->SetMobility(sender[22]);
dev[22]->SetAddress(Mac16Address("00:17"));
/*I9*/
sender[23]->SetPosition(Vector(81.9,13.6,0));
dev[23]->GetPhy()->SetMobility(sender[23]);
dev[23]->SetAddress(Mac16Address("00:18"));
/*I10*/
sender[24]->SetPosition(Vector(76.8,18.7,0));
dev[24]->GetPhy()->SetMobility(sender[24]);
dev[24]->SetAddress(Mac16Address("00:19"));
/*I11*/
sender[25]->SetPosition(Vector(81.3,19.4,0));
dev[25]->GetPhy()->SetMobility(sender[25]);
dev[25]->SetAddress(Mac16Address("00:1a"));
/*I12*/
sender[26]->SetPosition(Vector(85.3,17.3,0));
dev[26]->GetPhy()->SetMobility(sender[26]);
dev[26]->SetAddress(Mac16Address("00:1b"));


/*K*/
sender[27]->SetPosition(Vector(102,15.3,0));
dev[27]->GetPhy()->SetMobility(sender[27]);
dev[27]->SetAddress(Mac16Address("00:1c"));
/*K1*/
sender[28]->SetPosition(Vector(97.9,18.7,0));
dev[28]->GetPhy()->SetMobility(sender[28]);
dev[28]->SetAddress(Mac16Address("00:1d"));
/*K2*/
sender[29]->SetPosition(Vector(93.8,19.4,0));
dev[29]->GetPhy()->SetMobility(sender[29]);
dev[29]->SetAddress(Mac16Address("00:1e"));
/*K3*/
sender[30]->SetPosition(Vector(89.4,17.3,0));
dev[30]->GetPhy()->SetMobility(sender[30]);
dev[30]->SetAddress(Mac16Address("00:1f"));
/*K4*/
sender[31]->SetPosition(Vector(96.2,13.6,0));
dev[31]->GetPhy()->SetMobility(sender[31]);
dev[31]->SetAddress(Mac16Address("00:20"));
/*K5*/
sender[32]->SetPosition(Vector(93.5,13.6,0));
dev[32]->GetPhy()->SetMobility(sender[32]);
dev[32]->SetAddress(Mac16Address("00:21"));
/*K6*/
sender[33]->SetPosition(Vector(94.9,6.5,0));
dev[33]->GetPhy()->SetMobility(sender[33]);
dev[33]->SetAddress(Mac16Address("00:22"));
/*K7*/
sender[34]->SetPosition(Vector(109.5,6.5,0));
dev[34]->GetPhy()->SetMobility(sender[34]);
dev[34]->SetAddress(Mac16Address("00:23"));
/*K9*/
sender[35]->SetPosition(Vector(107.8,13.6,0));
dev[35]->GetPhy()->SetMobility(sender[35]);
dev[35]->SetAddress(Mac16Address("00:24"));
/*K10*/
sender[36]->SetPosition(Vector(110.5,13.6,0));
dev[36]->GetPhy()->SetMobility(sender[36]);
dev[36]->SetAddress(Mac16Address("00:25"));
/*K11*/
sender[37]->SetPosition(Vector(104.7,17.3,0));
dev[37]->GetPhy()->SetMobility(sender[37]);
dev[37]->SetAddress(Mac16Address("00:26"));
/*K12*/
sender[38]->SetPosition(Vector(108.1,19.4,0));
dev[38]->GetPhy()->SetMobility(sender[38]);
dev[38]->SetAddress(Mac16Address("00:27"));
/*K13*/
sender[39]->SetPosition(Vector(112.2,18.7,0));
dev[39]->GetPhy()->SetMobility(sender[39]);
dev[39]->SetAddress(Mac16Address("00:28"));


/*Q*/
sender[40]->SetPosition(Vector(44.5,-31,0));
dev[40]->GetPhy()->SetMobility(sender[40]);
dev[40]->SetAddress(Mac16Address("00:29"));
/*Q1*/
sender[41]->SetPosition(Vector(40.5,-23.5,0));
dev[41]->GetPhy()->SetMobility(sender[41]);
dev[41]->SetAddress(Mac16Address("00:2a"));
/*Q2*/
sender[42]->SetPosition(Vector(35.4,-29.6,0));
dev[42]->GetPhy()->SetMobility(sender[42]);
dev[42]->SetAddress(Mac16Address("00:2b"));
/*Q3*/
sender[43]->SetPosition(Vector(38.4,-29.6,0));
dev[43]->GetPhy()->SetMobility(sender[43]);
dev[43]->SetAddress(Mac16Address("00:2c"));
/*Q4*/
sender[44]->SetPosition(Vector(36,-36.4,0));
dev[44]->GetPhy()->SetMobility(sender[44]);
dev[44]->SetAddress(Mac16Address("00:2d"));
/*Q5*/
sender[45]->SetPosition(Vector(40.8,-35,0));
dev[45]->GetPhy()->SetMobility(sender[45]);
dev[45]->SetAddress(Mac16Address("00:2e"));
/*Q6*/
sender[46]->SetPosition(Vector(40.8,-35,0));
dev[46]->GetPhy()->SetMobility(sender[46]);
dev[46]->SetAddress(Mac16Address("00:2f"));
/*Q7*/
sender[47]->SetPosition(Vector(49.6,-29.6,0));
dev[47]->GetPhy()->SetMobility(sender[47]);
dev[47]->SetAddress(Mac16Address("00:30"));
/*Q8(P1)*/
sender[48]->SetPosition(Vector(32.6,-34,0));
dev[48]->GetPhy()->SetMobility(sender[48]);
dev[48]->SetAddress(Mac16Address("00:31"));
/*Q9(R1)*/
sender[49]->SetPosition(Vector(51.3,-23.5,0));
dev[49]->GetPhy()->SetMobility(sender[49]);
dev[49]->SetAddress(Mac16Address("00:32"));
/*Q10(R2)*/
sender[50]->SetPosition(Vector(52.7,-29.6,0));
dev[50]->GetPhy()->SetMobility(sender[50]);
dev[50]->SetAddress(Mac16Address("00:33"));
/*Q11(R3)*/
sender[51]->SetPosition(Vector(52,-36.4,0));
dev[51]->GetPhy()->SetMobility(sender[51]);
dev[51]->SetAddress(Mac16Address("00:34"));
/*Q12(R4)*/
sender[52]->SetPosition(Vector(55.8,-34,0));
dev[52]->GetPhy()->SetMobility(sender[52]);
dev[52]->SetAddress(Mac16Address("00:35"));

/*R*/
sender[53]->SetPosition(Vector(58.8,-31,0));
dev[53]->GetPhy()->SetMobility(sender[53]);
dev[53]->SetAddress(Mac16Address("00:36"));
/*R1(S3)*/
sender[54]->SetPosition(Vector(64.3,-36.4,0));
dev[54]->GetPhy()->SetMobility(sender[54]);
dev[54]->SetAddress(Mac16Address("00:37"));
///////////////////////////////////////////////////
/*R2(T1)*/
sender[55]->SetPosition(Vector(85,-34,0));
dev[55]->GetPhy()->SetMobility(sender[55]);
dev[55]->SetAddress(Mac16Address("00:38"));
/*R3(T2)*/
sender[56]->SetPosition(Vector(81.2,-36.4,0));
dev[56]->GetPhy()->SetMobility(sender[56]);
dev[56]->SetAddress(Mac16Address("00:39"));
/*R4(T3)*/
sender[57]->SetPosition(Vector(77.5,-34,0));
dev[57]->GetPhy()->SetMobility(sender[57]);
dev[57]->SetAddress(Mac16Address("00:3a"));
///////////////////////////////////////////////////
/*R5*/
sender[58]->SetPosition(Vector(61.2,-34,0));
dev[58]->GetPhy()->SetMobility(sender[58]);
dev[58]->SetAddress(Mac16Address("00:3b"));
/*R6*/
sender[59]->SetPosition(Vector(68.7,-34,0));
dev[59]->GetPhy()->SetMobility(sender[59]);
dev[59]->SetAddress(Mac16Address("00:3c"));
/*R7*/
sender[60]->SetPosition(Vector(63.9,-29.6,0));
dev[60]->GetPhy()->SetMobility(sender[60]);
dev[60]->SetAddress(Mac16Address("00:3d"));
/*R8*/
sender[61]->SetPosition(Vector(67,-29.6,0));
dev[61]->GetPhy()->SetMobility(sender[61]);
dev[61]->SetAddress(Mac16Address("00:3e"));
/*R9*/
sender[62]->SetPosition(Vector(65.6,-23.5,0));
dev[62]->GetPhy()->SetMobility(sender[62]);
dev[62]->SetAddress(Mac16Address("00:3f"));
/*R10(T4)*/
sender[63]->SetPosition(Vector(78.9,-29.6,0));
dev[63]->GetPhy()->SetMobility(sender[63]);
dev[63]->SetAddress(Mac16Address("00:40"));
/*R11(T5)*/
sender[64]->SetPosition(Vector(82,-29.6,0));
dev[64]->GetPhy()->SetMobility(sender[64]);
dev[64]->SetAddress(Mac16Address("00:41"));
/*R12(T6)*/
sender[65]->SetPosition(Vector(80.2,-23.5,0));
dev[65]->GetPhy()->SetMobility(sender[65]);
dev[65]->SetAddress(Mac16Address("00:42"));
///////////////////////////////////////////////////////////////////
/*T*/
sender[66]->SetPosition(Vector(100,-31,0));
dev[66]->GetPhy()->SetMobility(sender[66]);
dev[66]->SetAddress(Mac16Address("00:43"));
/*T7*/
sender[67]->SetPosition(Vector(94.5,-23.5,0));
dev[67]->GetPhy()->SetMobility(sender[67]);
dev[67]->SetAddress(Mac16Address("00:44"));
/*T8*/
sender[68]->SetPosition(Vector(93.2,-29.6,0));
dev[68]->GetPhy()->SetMobility(sender[68]);
dev[68]->SetAddress(Mac16Address("00:45"));
/*T9*/
sender[69]->SetPosition(Vector(96.2,-29.6,0));
dev[69]->GetPhy()->SetMobility(sender[69]);
dev[69]->SetAddress(Mac16Address("00:46"));
/*T10*/
sender[70]->SetPosition(Vector(90.8,-34,0));
dev[70]->GetPhy()->SetMobility(sender[70]);
dev[70]->SetAddress(Mac16Address("00:47"));
/*U1*/
sender[71]->SetPosition(Vector(95.5,-36.4,0));
dev[71]->GetPhy()->SetMobility(sender[71]);
dev[71]->SetAddress(Mac16Address("00:48"));
/*U2*/
sender[72]->SetPosition(Vector(99.6,-34,0));
dev[72]->GetPhy()->SetMobility(sender[72]);
dev[72]->SetAddress(Mac16Address("00:49"));
///NEW
/*T11*/
sender[73]->SetPosition(Vector(106.5,-23.5,0));
dev[73]->GetPhy()->SetMobility(sender[73]);
dev[73]->SetAddress(Mac16Address("00:4a"));
/*T12*/
sender[74]->SetPosition(Vector(106.8,-29.6,0));
dev[74]->GetPhy()->SetMobility(sender[74]);
dev[74]->SetAddress(Mac16Address("00:4b"));
/*T13*/
sender[75]->SetPosition(Vector(103.8,-29.6,0));
dev[75]->GetPhy()->SetMobility(sender[75]);
dev[75]->SetAddress(Mac16Address("00:4c"));
/*T14*/
sender[76]->SetPosition(Vector(109.2,-34,0));
dev[76]->GetPhy()->SetMobility(sender[76]);
dev[76]->SetAddress(Mac16Address("00:4d"));
/*T15*/
sender[77]->SetPosition(Vector(104.5,-36.4,0));
dev[77]->GetPhy()->SetMobility(sender[77]);
dev[77]->SetAddress(Mac16Address("00:4e"));
/*T16*/
sender[78]->SetPosition(Vector(100.4,-34,0));
dev[78]->GetPhy()->SetMobility(sender[78]);
dev[78]->SetAddress(Mac16Address("00:4f"));

//FINAL ROOT:
sender[79]->SetPosition(Vector(0,0,0));
dev[79]->GetPhy()->SetMobility(sender[79]);
dev[79]->SetAddress(Mac16Address("00:50"));

//EXTRA Nodes(silos):
sender[80]->SetPosition(Vector(0,-10,0));
dev[80]->GetPhy()->SetMobility(sender[80]);
dev[80]->SetAddress(Mac16Address("00:51"));

sender[81]->SetPosition(Vector(0,-15,0));
dev[81]->GetPhy()->SetMobility(sender[81]);
dev[81]->SetAddress(Mac16Address("00:52"));

sender[82]->SetPosition(Vector(0,-20,0));
dev[82]->GetPhy()->SetMobility(sender[82]);
dev[82]->SetAddress(Mac16Address("00:53"));

sender[83]->SetPosition(Vector(0,-25,0));
dev[83]->GetPhy()->SetMobility(sender[83]);
dev[83]->SetAddress(Mac16Address("00:54"));
//..
//Install protocol stack at the nodes
InternetStackHelper internetv6;
internetv6.Install (nodes);
//install the 6LoWPAN element to the nodes
SixLowPanHelper sixlowpan;
NetDeviceContainer devices = sixlowpan.Install (lrwpanDevices);

//set IPv6 addresses:
Ipv6AddressHelper ipv6;
ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));

Ipv6InterfaceContainer deviceInterfaces;
deviceInterfaces = ipv6.Assign (devices);
//loop over for other stuff
for(iterator = 0; iterator < 85; iterator++){
	deviceInterfaces.SetForwarding (iterator, true);
}
deviceInterfaces.SetDefaultRouteInAllNodes (0); //I forget what this is for
//

///up to L173///
///Static routing stuff here

Ptr<Ipv6> ipv6Array[85];
for(iterator = 0; iterator < 85; iterator++){
	ipv6Array[iterator] = allNodes[iterator]->GetObject<Ipv6> ();
	
}

NS_LOG_UNCOND("Just before routing\n");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//KMG: Likewise, populating the lookup tables in a loop resulted in a difficult run-time error => manually code them
	//ROOT:
//uint16_t one = 1;
//uint16_t counter;
uint16_t innerCount;
//uint16_t step = 13;
//////////////////////
//G:
//////////////////////
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //ROOT to FFD
dev[1]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());//FFD to ROOT
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(3)); //ROOT to RFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(3)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[2]->GetMac()->GetShortAddress()),uint16_t(3)); //FFD to RFD
dev[2]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[2]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(4)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[3]->GetMac()->GetShortAddress()),uint16_t(4)); //FFD to RFD
dev[3]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[3]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(5)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[4]->GetMac()->GetShortAddress()),uint16_t(5)); //FFD to RFD
dev[4]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[4]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(6)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[5]->GetMac()->GetShortAddress()),uint16_t(6)); //FFD to RFD
dev[5]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[5]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(7)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[6]->GetMac()->GetShortAddress()),uint16_t(7)); //FFD to RFD
dev[6]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[6]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(8)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[7]->GetMac()->GetShortAddress()),uint16_t(8)); //FFD to RFD
dev[7]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[7]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(9)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[8]->GetMac()->GetShortAddress()),uint16_t(9)); //FFD to RFD
dev[8]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[8]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(10)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[9]->GetMac()->GetShortAddress()),uint16_t(10)); //FFD to RFD
dev[9]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[9]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(11)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[10]->GetMac()->GetShortAddress()),uint16_t(11)); //FFD to RFD
dev[10]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[10]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(12)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[11]->GetMac()->GetShortAddress()),uint16_t(12)); //FFD to RFD
dev[11]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[11]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(13)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[12]->GetMac()->GetShortAddress()),uint16_t(13)); //FFD to RFD
dev[12]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[12]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(14)); //ROOT to RFD
dev[1]->GetMac()->SetNextHopForIndex((dev[13]->GetMac()->GetShortAddress()),uint16_t(14)); //FFD to RFD
dev[13]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[13]->GetMac()->SetNextHopForIndex((dev[1]->GetMac()->GetShortAddress()),uint16_t(2)); //RFD to FFD
//////////////////////
//I:
//////////////////////
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //ROOT to FFD
dev[14]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());//FFD to ROOT
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(16)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[15]->GetMac()->GetShortAddress()),uint16_t(16)); //FFD to RFD
dev[15]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[15]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(17)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[16]->GetMac()->GetShortAddress()),uint16_t(17)); //FFD to RFD
dev[16]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[16]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(18)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[17]->GetMac()->GetShortAddress()),uint16_t(18)); //FFD to RFD
dev[17]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[17]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(19)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[18]->GetMac()->GetShortAddress()),uint16_t(19)); //FFD to RFD
dev[18]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[18]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(20)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[19]->GetMac()->GetShortAddress()),uint16_t(20)); //FFD to RFD
dev[19]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[19]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(21)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[20]->GetMac()->GetShortAddress()),uint16_t(21)); //FFD to RFD
dev[20]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[20]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(22)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[21]->GetMac()->GetShortAddress()),uint16_t(22)); //FFD to RFD
dev[21]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[21]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(23)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[22]->GetMac()->GetShortAddress()),uint16_t(23)); //FFD to RFD
dev[22]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[22]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(24)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[23]->GetMac()->GetShortAddress()),uint16_t(24)); //FFD to RFD
dev[23]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[23]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(25)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[24]->GetMac()->GetShortAddress()),uint16_t(25)); //FFD to RFD
dev[24]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[24]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(26)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[25]->GetMac()->GetShortAddress()),uint16_t(26)); //FFD to RFD
dev[25]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[25]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(27)); //ROOT to RFD
dev[14]->GetMac()->SetNextHopForIndex((dev[26]->GetMac()->GetShortAddress()),uint16_t(27)); //FFD to RFD
dev[26]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[26]->GetMac()->SetNextHopForIndex((dev[14]->GetMac()->GetShortAddress()),uint16_t(15)); //RFD to FFD
//////////////////////
//K:
//////////////////////
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //ROOT to FFD
dev[27]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());//FFD to ROOT
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(29)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[28]->GetMac()->GetShortAddress()),uint16_t(29)); //FFD to RFD
dev[28]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[28]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(30)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[29]->GetMac()->GetShortAddress()),uint16_t(30)); //FFD to RFD
dev[29]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[29]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(31)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[30]->GetMac()->GetShortAddress()),uint16_t(31)); //FFD to RFD
dev[30]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[30]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(32)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[31]->GetMac()->GetShortAddress()),uint16_t(32)); //FFD to RFD
dev[31]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[31]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(33)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[32]->GetMac()->GetShortAddress()),uint16_t(33)); //FFD to RFD
dev[32]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[32]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(34)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[33]->GetMac()->GetShortAddress()),uint16_t(34)); //FFD to RFD
dev[33]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[33]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(35)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[34]->GetMac()->GetShortAddress()),uint16_t(35)); //FFD to RFD
dev[34]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[34]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(36)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[35]->GetMac()->GetShortAddress()),uint16_t(36)); //FFD to RFD
dev[35]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[35]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(37)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[36]->GetMac()->GetShortAddress()),uint16_t(37)); //FFD to RFD
dev[36]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[36]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(38)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[37]->GetMac()->GetShortAddress()),uint16_t(38)); //FFD to RFD
dev[37]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[37]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(39)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[38]->GetMac()->GetShortAddress()),uint16_t(39)); //FFD to RFD
dev[38]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[38]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(40)); //ROOT to RFD
dev[27]->GetMac()->SetNextHopForIndex((dev[39]->GetMac()->GetShortAddress()),uint16_t(40)); //FFD to RFD
dev[39]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[39]->GetMac()->SetNextHopForIndex((dev[27]->GetMac()->GetShortAddress()),uint16_t(28)); //RFD to FFD
//////////////////////
//Q:
//////////////////////
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //ROOT to FFD
dev[40]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());//FFD to ROOT
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(42)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[41]->GetMac()->GetShortAddress()),uint16_t(42)); //FFD to RFD
dev[41]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[41]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(43)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[42]->GetMac()->GetShortAddress()),uint16_t(43)); //FFD to RFD
dev[42]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[42]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(44)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[43]->GetMac()->GetShortAddress()),uint16_t(44)); //FFD to RFD
dev[43]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[43]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(45)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[44]->GetMac()->GetShortAddress()),uint16_t(45)); //FFD to RFD
dev[44]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[44]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(46)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[45]->GetMac()->GetShortAddress()),uint16_t(46)); //FFD to RFD
dev[45]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[45]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(47)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[46]->GetMac()->GetShortAddress()),uint16_t(47)); //FFD to RFD
dev[46]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[46]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(48)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[47]->GetMac()->GetShortAddress()),uint16_t(48)); //FFD to RFD
dev[47]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[47]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(49)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[48]->GetMac()->GetShortAddress()),uint16_t(49)); //FFD to RFD
dev[48]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[48]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(50)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[49]->GetMac()->GetShortAddress()),uint16_t(50)); //FFD to RFD
dev[49]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[49]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(51)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[50]->GetMac()->GetShortAddress()),uint16_t(51)); //FFD to RFD
dev[50]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[50]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(52)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[51]->GetMac()->GetShortAddress()),uint16_t(52)); //FFD to RFD
dev[51]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[51]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(53)); //ROOT to RFD
dev[40]->GetMac()->SetNextHopForIndex((dev[52]->GetMac()->GetShortAddress()),uint16_t(53)); //FFD to RFD
dev[52]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[52]->GetMac()->SetNextHopForIndex((dev[40]->GetMac()->GetShortAddress()),uint16_t(41)); //RFD to FFD
//////////////////////
//R:
//////////////////////
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //ROOT to FFD
dev[53]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());//FFD to ROOT
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(55)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[54]->GetMac()->GetShortAddress()),uint16_t(55)); //FFD to RFD
dev[54]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[54]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(56)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[55]->GetMac()->GetShortAddress()),uint16_t(56)); //FFD to RFD
dev[55]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[55]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(57)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[56]->GetMac()->GetShortAddress()),uint16_t(57)); //FFD to RFD
dev[56]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[56]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(58)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[57]->GetMac()->GetShortAddress()),uint16_t(58)); //FFD to RFD
dev[57]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[57]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(59)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[58]->GetMac()->GetShortAddress()),uint16_t(59)); //FFD to RFD
dev[58]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[58]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(60)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[59]->GetMac()->GetShortAddress()),uint16_t(60)); //FFD to RFD
dev[59]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[59]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(61)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[60]->GetMac()->GetShortAddress()),uint16_t(61)); //FFD to RFD
dev[60]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[60]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(62)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[61]->GetMac()->GetShortAddress()),uint16_t(62)); //FFD to RFD
dev[61]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[61]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(63)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[62]->GetMac()->GetShortAddress()),uint16_t(63)); //FFD to RFD
dev[62]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[62]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(64)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[63]->GetMac()->GetShortAddress()),uint16_t(64)); //FFD to RFD
dev[63]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[63]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(65)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(65)); //FFD to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[65]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(66)); //ROOT to RFD
dev[53]->GetMac()->SetNextHopForIndex((dev[66]->GetMac()->GetShortAddress()),uint16_t(66)); //FFD to RFD
dev[66]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[66]->GetMac()->SetNextHopForIndex((dev[53]->GetMac()->GetShortAddress()),uint16_t(54)); //RFD to FFD
//////////////////////
//T:
//////////////////////
dev[0]->GetMac()->SetNextHopForIndex((dev[67]->GetMac()->GetShortAddress()),uint16_t(68)); //ROOT to FFD
dev[67]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());//FFD to ROOT
//
dev[0]->GetMac()->SetNextHopForIndex((dev[67]->GetMac()->GetShortAddress()),uint16_t(67)); //ROOT to RFD
dev[67]->GetMac()->SetNextHopForIndex((dev[66]->GetMac()->GetShortAddress()),uint16_t(67)); //FFD to RFD
dev[66]->GetMac()->SetNextHopForIndex((dev[67]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[66]->GetMac()->SetNextHopForIndex((dev[67]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(68)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[67]->GetMac()->GetShortAddress()),uint16_t(68)); //FFD to RFD
dev[67]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[67]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(69)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[68]->GetMac()->GetShortAddress()),uint16_t(69)); //FFD to RFD
dev[68]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[68]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(70)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[69]->GetMac()->GetShortAddress()),uint16_t(70)); //FFD to RFD
dev[69]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[69]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(71)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[70]->GetMac()->GetShortAddress()),uint16_t(71)); //FFD to RFD
dev[70]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[70]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(72)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[71]->GetMac()->GetShortAddress()),uint16_t(72)); //FFD to RFD
dev[71]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[71]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(73)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[72]->GetMac()->GetShortAddress()),uint16_t(73)); //FFD to RFD
dev[72]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[72]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(74)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[73]->GetMac()->GetShortAddress()),uint16_t(74)); //FFD to RFD
dev[73]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[73]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(75)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[74]->GetMac()->GetShortAddress()),uint16_t(75)); //FFD to RFD
dev[74]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[74]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(66)); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(76)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[75]->GetMac()->GetShortAddress()),uint16_t(76)); //FFD to RFD
dev[75]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[75]->GetMac()->SetMeshParentAddress((dev[65]->GetMac()->GetShortAddress())); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(77)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[76]->GetMac()->GetShortAddress()),uint16_t(77)); //FFD to RFD
dev[76]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[76]->GetMac()->SetMeshParentAddress((dev[65]->GetMac()->GetShortAddress())); //RFD to FFD
//
dev[0]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(78)); //ROOT to RFD
dev[65]->GetMac()->SetNextHopForIndex((dev[77]->GetMac()->GetShortAddress()),uint16_t(78)); //FFD to RFD
dev[77]->GetMac()->SetNextHopForIndex((dev[65]->GetMac()->GetShortAddress()),uint16_t(1)); //RFD to ROOT
dev[77]->GetMac()->SetMeshParentAddress((dev[65]->GetMac()->GetShortAddress())); //RFD to FFD
///////
//original loop that resulted in memory leak issues(used hard coding instead)
/*
for(counter = 0; counter < 6; counter++){

	// FFD to ROOT
	dev[(counter*step)+1]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());
	dev[0]->GetMac()->SetNextHopForIndex((dev[(counter*step)+1]->GetMac()->GetShortAddress()),((counter*step)+1+1));
	for(innerCount = 0;innerCount < 12;innerCount++){
		//RFD to FFD parent
		dev[(counter*step)+2+innerCount]->GetMac()->SetMeshParentAddress(dev[((counter*step)+1)]->GetMac()->GetShortAddress());
		//ROOT to RFD children
		dev[0]->GetMac()->SetNextHopForIndex(dev[((counter*step)+1)]->GetMac()->GetShortAddress(),uint16_t((counter*step)+2+innerCount+1));
		//FFD to RFD children
		dev[((counter*step)+1)]->GetMac()->SetNextHopForIndex(dev[((counter*step)+2+innerCount)]->GetMac()->GetShortAddress(),uint16_t((counter*step)+2+innerCount+1));
	}

}
*/
////lets add the newer root node(and some nearby RFDs):

dev[78]->GetMac()->SetMeshParentAddress(dev[0]->GetMac()->GetShortAddress());
dev[0]->GetMac()->SetMeshParentAddress(dev[78]->GetMac()->GetShortAddress());
//RFDs:

for(innerCount = 79; innerCount < 83; innerCount++){
	dev[78]->GetMac()->SetNextHopForIndex(dev[innerCount]->GetMac()->GetShortAddress(),uint16_t(innerCount));
	dev[innerCount]->GetMac()->SetMeshParentAddress(dev[78]->GetMac()->GetShortAddress());

}

////////////////////////////////////////////////////// Application Layer ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
NS_LOG_UNCOND("Adding PING layer.\n");
uint32_t packetSize = 400; //bytes
Time interPacketInterval = Seconds (1);//Seconds (0.1);
uint32_t numPackets = 10; // packets(This was modified between simulations, where necessary)
Ping6Helper ping6_SRC[80];
ApplicationContainer apps_SRC[80];
uint16_t srcCount = 0;

for(innerCount = 2; innerCount < 14; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (2.0));
	apps_SRC[srcCount].Stop (Seconds (120.0));
	srcCount++;
}

for(innerCount = 15; innerCount < 27; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (20.0));
	apps_SRC[srcCount].Stop (Seconds (120.0));
	srcCount++;
}

for(innerCount = 28; innerCount < 40; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (40.0));
	apps_SRC[srcCount].Stop (Seconds (120.0));
	srcCount++;
}

for(innerCount = 41; innerCount < 53; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (60.0));
	apps_SRC[srcCount].Stop (Seconds (120.0));
	srcCount++;
}

for(innerCount = 54; innerCount < 67; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (80.0));
	apps_SRC[srcCount].Stop (Seconds (120.0));
	srcCount++;
}

for(innerCount = 66; innerCount < 78; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (100.0));
	apps_SRC[srcCount].Stop (Seconds (120.0));
	srcCount++;
}

for(innerCount = 79; innerCount < 83; innerCount++){
	ping6_SRC[srcCount].SetLocal (deviceInterfaces.GetAddress (innerCount, 1));
	ping6_SRC[srcCount].SetRemote (deviceInterfaces.GetAddress (0, 1));
	ping6_SRC[srcCount].SetAttribute ("MaxPackets", UintegerValue (numPackets));
	ping6_SRC[srcCount].SetAttribute ("Interval", TimeValue (interPacketInterval));
	ping6_SRC[srcCount].SetAttribute ("PacketSize", UintegerValue (packetSize));
	apps_SRC[srcCount] = ping6_SRC[srcCount].Install (nodes.Get (innerCount));
	apps_SRC[srcCount].Start (Seconds (120.0));
	apps_SRC[srcCount].Stop (Seconds (140.0));
	srcCount++;
}

NS_LOG_UNCOND("End of PING stuff.\n");
///PCAP traces:
//Layer 3 PCAP
internetv6.EnablePcapIpv6All(std::string ("normalV6_mUnder"));
//Layer 2 PCAP
lrWpanHelper.EnablePcap(std::string ("normalL2_mUnder"),lrwpanDevices, true);

//Mobility - Node location
MobilityHelper mobility;

Ptr<ListPositionAllocator> positionAlloc = CreateObject
<ListPositionAllocator>();
mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

mobility.Install(nodes);
///Some display stuff
AnimationInterface anim ("wsn_main.xml"); // Mandatory
anim.EnablePacketMetadata(true);


//run the simulation:
NS_LOG_UNCOND("Run Simulation.");
//Not UDP!!!


// Calculate Throughput using Flowmonitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();

// Now, do the actual simulation.
Simulator::Stop (Seconds(150.0));
Simulator::Run ();




//End:
Simulator::Destroy ();
NS_LOG_INFO ("Done.");
return 0;
}
