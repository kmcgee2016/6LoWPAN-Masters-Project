//Author: Kevin Mc Gee, Based on original work(L4 routing) by Moab Rodrigues de Jesus
/*
// Network topology:
//
// n0 		n1 		n2 		n3		n4
// |		|		|		| 		| 
// ===================================
// <--------><------><------><------>
//	   25m		25m		25m		25m
// WSN (802.15.4)
*/

//Based on the examples wsn-ping6.cc, ping6.cc, example-ping-lr-wpan.cc and
//udp-echo.cc

#include "ns3/core-module.h"
#include "ns3/lr-wpan-module.h" //my stuff should be in here too!
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
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv6-flow-classifier.h"
#include "ns3/ping6-helper.h"
//#include "lowpanL2Hdr.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("WSN");//TODO: See what the deal is with this stuff
//LogComponentEnable("lr-wpan-mac", LOG_LEVEL_ALL)
//callback for packet reception
void ReceivePacket (Ptr<Socket> dest, Ptr<Socket> socket){
	//dest->Connect(remote);
	while (socket->Recv ()){
		NS_LOG_UNCOND ("Node " << socket->GetNode()->GetId() << " received one packet!");
		dest->Send (Create<Packet>());
	}
	//dest->Close();
}



void ReceiveDestiny (Ptr<Socket> socket){
	while (socket->Recv ()){
		NS_LOG_UNCOND ("Package arrived at no 3");
	}
}



static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval ){
	if (pktCount > 0){
		socket->Send (Create<Packet> (pktSize));
		NS_LOG_UNCOND("Node 0 sends packet number "<< pktCount);
		Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize,pktCount-1, pktInterval);
	}else{
		socket->Close ();
	}
}



int main(){
// Enable calculation of FCS in the trailers. Only necessary when interacting with real devices or wireshark.(Tommaso Pecorella)
GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

LogComponentEnable ("WSN", LOG_LEVEL_INFO);	
LogComponentEnable ("Socket", LOG_LEVEL_INFO);
//lowpanL2Hdr myHeader;


//Create 3 nodes, and a device for each one
Ptr<Node> n0 = CreateObject<Node>();
Ptr<Node> n1 = CreateObject<Node>();
Ptr<Node> n2 = CreateObject<Node>();
Ptr<Node> n3 = CreateObject<Node>();
Ptr<Node> n4 = CreateObject<Node>();
Ptr<Node> n5 = CreateObject<Node>();
Ptr<Node> n6 = CreateObject<Node>();

//deprecated stuff for channel tuning
double txPower = -6;
uint32_t channelNumber = 11;

//add the nodes to a container
NodeContainer nodes;
nodes.Add(n0);
nodes.Add(n1);
nodes.Add(n2);
nodes.Add(n3);
nodes.Add(n4);
nodes.Add(n5);
nodes.Add(n6);

//create several LrWpanNetDevice pointers
Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev2 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev3 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev4 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev5 = CreateObject<LrWpanNetDevice>();
Ptr<LrWpanNetDevice> dev6 = CreateObject<LrWpanNetDevice>();

//NetDeviceContainer lrwpanDevices = NetDeviceContainer(dev0, dev1, dev2, dev3);
LrWpanHelper lrWpanHelper; //use helper later on to enable Layer 2 PCAP traces
//create a device container for the lrWpanNetDevice nodes
NetDeviceContainer lrwpanDevices;

lrwpanDevices.Add(dev0);
lrwpanDevices.Add(dev1);
lrwpanDevices.Add(dev2);
lrwpanDevices.Add(dev3);
lrwpanDevices.Add(dev4);
lrwpanDevices.Add(dev5);
lrwpanDevices.Add(dev6);

//std::cout<<lrwpanDevices.GetN();
//assign short MAC addresses to each of the LrWpanNetDevices
dev0->SetAddress(Mac16Address("00:01"));
dev1->SetAddress(Mac16Address("00:02"));
dev2->SetAddress(Mac16Address("00:03"));
dev3->SetAddress(Mac16Address("00:04"));
dev4->SetAddress(Mac16Address("00:05"));
dev5->SetAddress(Mac16Address("00:06"));
dev6->SetAddress(Mac16Address("00:07"));

dev0->GetMac()->SetMeshParentAddress(Mac16Address("00:02"));
dev1->GetMac()->SetMeshParentAddress(Mac16Address("00:03"));
dev2->GetMac()->SetMeshParentAddress(Mac16Address("00:04"));
dev3->GetMac()->SetMeshParentAddress(Mac16Address("00:05"));
dev4->GetMac()->SetMeshParentAddress(Mac16Address("00:04"));

//populate the MAC addresses properly for the dst. nodes:
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:01"), uint16_t(1));//Node 1 -> Node 1
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(2));//Node 1 -> Node 2
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(3));//Node 1 -> Node 3 via Node 2
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(4));//Node 1 -> Node 4 via Node 2
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(5));//Node 1 -> Node 5 via Node 2
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(6));//Node 1 -> Node 5 via Node 2
dev0->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(7));//Node 1 -> Node 5 via Node 2

dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:01"), uint16_t(1));//Node 2 -> Node 1
dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(2));//Node 2 -> Node 2
dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(3));//Node 2 -> Node 3
dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(4));//Node 2 -> Node 4 via Node 3
dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(5));//Node 2 -> Node 5 via Node 3
dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(6));//Node 2 -> Node 5 via Node 3
dev1->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(7));//Node 2 -> Node 5 via Node 3

dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(1));//Node 3 -> Node 1 via Node 2
dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:02"), uint16_t(2));//Node 3 -> Node 2
dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(3));//Node 3 -> Node 3
dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(4));//Node 3 -> Node 4
dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(5));//Node 3 -> Node 5 via Node 4
dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(6));//Node 3 -> Node 5 via Node 4
dev2->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(7));//Node 3 -> Node 5 via Node 4

dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(1));//Node 4 -> Node 1 via Node 3
dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(2));//Node 4 -> Node 2 via Node 3
dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:03"), uint16_t(3));//Node 4 -> Node 3
dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(4));//Node 4 -> Node 4
dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(5));//Node 4 -> Node 5
dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(6));//Node 4 -> Node 5 via Node 5
dev3->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(7));//Node 4 -> Node 5 via Node 5

dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(1));//Node 5 -> Node 1 via Node 4
dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(2));//Node 5 -> Node 2 via Node 4
dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(3));//Node 5 -> Node 3 via Node 4
dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:04"), uint16_t(4));//Node 5 -> Node 4
dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(5));//Node 5 -> Node 5
dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(6));//Node 5 -> Node 5 via Node 6
dev4->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(7));//Node 5 -> Node 5 via Node 6

dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(1));//Node 6 -> Node 1 via Node 4
dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(2));//Node 6 -> Node 2 via Node 4
dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(3));//Node 6 -> Node 3 via Node 4
dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(4));//Node 6 -> Node 4
dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:05"), uint16_t(5));//Node 6 -> Node 5
dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(6));//Node 6 -> Node 6
dev5->GetMac()->SetNextHopForIndex(Mac16Address("00:07"), uint16_t(7));//Node 6 -> Node 7

dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(1));//Node 7 -> Node 1 via Node 4
dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(2));//Node 7 -> Node 2 via Node 4
dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(3));//Node 7 -> Node 3 via Node 4
dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(4));//Node 7 -> Node 4
dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(5));//Node 7 -> Node 5
dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:06"), uint16_t(6));//Node 7 -> Node 6
dev6->GetMac()->SetNextHopForIndex(Mac16Address("00:07"), uint16_t(7));//Node 7 -> Node 7

///Print out of the routing table:
std::string tables;
for(uint16_t i = 1; i < 6; i++){
	NS_LOG_UNCOND("Device dev0"  << " entry " << i << " equals: " << dev0->GetMac()->GetNextHopForIndex(i));
	NS_LOG_UNCOND("Device dev1"  << " entry " << i << " equals: " << dev1->GetMac()->GetNextHopForIndex(i));
	NS_LOG_UNCOND("Device dev2" << " entry " << i << " equals: " << dev2->GetMac()->GetNextHopForIndex(i));
	NS_LOG_UNCOND("Device dev3" << " entry " << i << " equals: " << dev3->GetMac()->GetNextHopForIndex(i));
	NS_LOG_UNCOND("Device dev4" << " entry " << i << " equals: " << dev4->GetMac()->GetNextHopForIndex(i));
}
///Current attempt at channel design - Needs overhaul
//Each device must be attached to same channel - 
Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>(); //maybe not the way to go. Look at 
// https://www.nsnam.org/doxygen/lr-wpan-error-distance-plot_8cc_source.html
Ptr<FriisPropagationLossModel> propModel = CreateObject<FriisPropagationLossModel>();//just for now
Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel>();//not sure what else to do
channel->AddPropagationLossModel(propModel);
channel->SetPropagationDelayModel(delayModel);

//set each device to have these channel characteristics
dev0->SetChannel(channel);
dev1->SetChannel(channel);
dev2->SetChannel(channel);
dev3->SetChannel(channel);
dev4->SetChannel(channel);
dev5->SetChannel(channel);
dev6->SetChannel(channel);

//To complete configuration, a LrWpanNetDevice must be added to a node
n0->AddDevice(dev0);
n1->AddDevice(dev1);
n2->AddDevice(dev2);
n3->AddDevice(dev3);
n4->AddDevice(dev4);
n5->AddDevice(dev5);
n6->AddDevice(dev6);

//I wish to check out what the deal is with the current error model:
Ptr<LrWpanErrorModel> newModel = CreateObject<LrWpanErrorModel>();
Ptr<LrWpanPhy> existingPhy = dev0->GetPhy();
existingPhy->SetErrorModel(newModel);

Ptr<ConstantPositionMobilityModel> sender0 = CreateObject<ConstantPositionMobilityModel>();
//sender0->SetPosition(Vector(0,0,0));
sender0->SetPosition(Vector(0,0,0));
dev0->GetPhy()->SetMobility(sender0);

Ptr<ConstantPositionMobilityModel> sender1 = CreateObject<ConstantPositionMobilityModel>();
//sender1->SetPosition(Vector(5,0,0));
sender1->SetPosition(Vector(25,0,0));
dev1->GetPhy()->SetMobility(sender1);

Ptr<ConstantPositionMobilityModel> sender2 = CreateObject<ConstantPositionMobilityModel>();
//sender2->SetPosition(Vector(10,0,0));
sender2->SetPosition(Vector(50,0,0));
dev2->GetPhy()->SetMobility(sender2);

Ptr<ConstantPositionMobilityModel> sender3 = CreateObject<ConstantPositionMobilityModel>();
//sender2->SetPosition(Vector(10,0,0));
sender3->SetPosition(Vector(75,0,0));
dev3->GetPhy()->SetMobility(sender3);

Ptr<ConstantPositionMobilityModel> sender4 = CreateObject<ConstantPositionMobilityModel>();
//sender2->SetPosition(Vector(10,0,0));
sender4->SetPosition(Vector(100,0,0));
dev4->GetPhy()->SetMobility(sender4);

Ptr<ConstantPositionMobilityModel> sender5 = CreateObject<ConstantPositionMobilityModel>();
//sender2->SetPosition(Vector(10,0,0));
sender5->SetPosition(Vector(125,0,0));
dev5->GetPhy()->SetMobility(sender5);

Ptr<ConstantPositionMobilityModel> sender6 = CreateObject<ConstantPositionMobilityModel>();
//sender2->SetPosition(Vector(10,0,0));
sender6->SetPosition(Vector(150,0,0));
dev6->GetPhy()->SetMobility(sender6);

//configure power of transmission - Needs overhaul
LrWpanSpectrumValueHelper svh;
Ptr<SpectrumValue> psd = svh.CreateTxPowerSpectralDensity (txPower, channelNumber);
//assign to the devices
dev0->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev1->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev2->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev3->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev4->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev5->GetPhy ()->SetTxPowerSpectralDensity (psd);
dev6->GetPhy ()->SetTxPowerSpectralDensity (psd);

//set the drop trace callback for Rx:


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
//allow each of the devices to act as a router
deviceInterfaces.SetForwarding (0, true);
deviceInterfaces.SetForwarding (1, true);
deviceInterfaces.SetForwarding (2, true);
deviceInterfaces.SetForwarding (3, true);
deviceInterfaces.SetForwarding (4, true);
deviceInterfaces.SetForwarding (5, true);
deviceInterfaces.SetForwarding (6, true);
deviceInterfaces.SetDefaultRouteInAllNodes (0); //I forget what this is for

//just print out the assigned addresses for reference
Address address0, address1, address2, address3, address4; //This is necessary for configure correctly the app layer.
address0 = Address(deviceInterfaces.GetAddress (0,1));
address1 = Address(deviceInterfaces.GetAddress (1,1));
address2 = Address(deviceInterfaces.GetAddress (2,1));
address3 = Address(deviceInterfaces.GetAddress (3,1));
address4 = Address(deviceInterfaces.GetAddress (4,1));
//Check the address of the nodes
std::cout<<address0<<std::endl;
std::cout<<address1<<std::endl;
std::cout<<address2<<std::endl;
std::cout<<address3<<std::endl;
std::cout<<address4<<std::endl;

///Static routing stuff here
Ptr<Ipv6> ipv6_0 = n0->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_1 = n1->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_2 = n2->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_3 = n3->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_4 = n4->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_5 = n5->GetObject<Ipv6> ();
Ptr<Ipv6> ipv6_6 = n6->GetObject<Ipv6> ();

//uint32_t one = 1; //Interface number
//uint32_t two = 2;
//Add static routing
NS_LOG_INFO ("Creating Static Routes.");



NS_LOG_INFO ("Adding PING layer.");
uint32_t packetSize = 1200; //bytes
Time interPacketInterval = Seconds (4);//Seconds (0.1);
uint32_t numPackets = 60; // packets
//TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
Ping6Helper ping6_SRC;
ping6_SRC.SetLocal (deviceInterfaces.GetAddress (0, 1));
ping6_SRC.SetRemote (deviceInterfaces.GetAddress (1, 1));
ping6_SRC.SetAttribute ("MaxPackets", UintegerValue (numPackets));
ping6_SRC.SetAttribute ("Interval", TimeValue (interPacketInterval));
ping6_SRC.SetAttribute ("PacketSize", UintegerValue (packetSize));
ApplicationContainer apps_SRC = ping6_SRC.Install (nodes.Get (0));
apps_SRC.Start (Seconds (2.0));
apps_SRC.Stop (Seconds (200.0));
NS_LOG_INFO ("PING set up - done");

///PCAP traces:
//Layer 3 PCAP
internetv6.EnablePcapIpv6All(std::string ("mUnder_V6_PING_ALT_PCAP-1200Bytes"));
//Layer 2 PCAP
lrWpanHelper.EnablePcap(std::string ("mUnder_L2_PING_ALT_PCAP-1200Bytes"),lrwpanDevices, true);

//Mobility - Node location
MobilityHelper mobility;

Ptr<ListPositionAllocator> positionAlloc = CreateObject
<ListPositionAllocator>();
mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

mobility.Install(nodes);
///Some display stuff
//AnimationInterface anim ("wsn_main.xml"); // Mandatory
//anim.EnablePacketMetadata(true);


//run the simulation:
NS_LOG_INFO ("Run Simulation.");
//Not:UDP!!!
//Simulator::ScheduleWithContext (source->GetNode ()->GetId (), Seconds (1.0), &GenerateTraffic, source, packetSize, numPackets, interPacketInterval);


// Calculate Throughput using Flowmonitor
FlowMonitorHelper flowmon;
Ptr<FlowMonitor> monitor = flowmon.InstallAll();

// Now, do the actual simulation.
Simulator::Stop (Seconds(220.0));
Simulator::Run ();

//stats:
monitor->CheckForLostPackets ();
Ptr<Ipv6FlowClassifier> classifier = DynamicCast<Ipv6FlowClassifier>(flowmon.GetClassifier ());
//std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
//std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();


//print out of results:
//std::cout << "Flow IP : (" << address0 << " -> " << address2 <<")\n";
//std::cout << " Tx Bytes: " << i->second.txBytes << "\n";
//std::cout << " Rx Bytes: " << i->second.rxBytes << "\n";
//std::cout << " Throughput: " << i->second.rxBytes * 8.0 /(i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024 << " Mbps\n";

//output:
monitor->SerializeToXmlFile("wsn_main.flowmon", true, true);


//End:
Simulator::Destroy ();
NS_LOG_INFO ("Done.");
return 0;
}

