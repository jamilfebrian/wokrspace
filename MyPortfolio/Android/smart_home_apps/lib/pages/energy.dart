import 'package:flutter/material.dart';
import 'package:my_app/widgets/services_card.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

class EnergyPage extends StatefulWidget {
  const EnergyPage({super.key});

  @override
  EnergyPageState createState() => EnergyPageState();
}

class EnergyPageState extends State<EnergyPage> {
  bool powerOnHeating = false;
  bool powerOnFan = false;
  bool powerOnRadio = false;
  bool powerOnTV = false;

  Future<void> kirimData(bool nilai, String device) async {
    Uri url = Uri.parse(
        "https://iot-project-1800-default-rtdb.firebaseio.com/$device/status.json");

    await http.put(
      url,
      headers: {"Content-Type": "application/json"},
      body: jsonEncode(nilai),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.indigo.shade50,
      body: SafeArea(
        child: Container(
          margin: const EdgeInsets.only(top: 18, left: 20, right: 20),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  GestureDetector(
                    onTap: () {
                      Navigator.pop(context);
                    },
                    child: const Icon(
                      Icons.arrow_back_ios,
                      color: Colors.indigo,
                    ),
                  ),
                  Text(
                    'Power Usage',
                    style: TextStyle(
                      fontSize: 24,
                      color: Colors.indigo.shade700,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  const RotatedBox(
                    quarterTurns: 135,
                    child: Icon(
                      Icons.bar_chart_rounded,
                      color: Colors.indigo,
                      size: 28,
                    ),
                  )
                ],
              ),
              Expanded(
                child: ListView(
                  physics: const BouncingScrollPhysics(),
                  children: [
                    const SizedBox(height: 32),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      children: [
                        SmartDeviceBox(
                          smartDeviceName: 'Lamp',
                          iconPath: 'assets/images/lamp.png',
                          powerOn: powerOnHeating,
                          onChanged: (value) {
                            setState(() {
                              powerOnHeating = value;
                              kirimData(powerOnHeating, "lamp");
                            });
                          },
                        ),
                        SmartDeviceBox(
                          smartDeviceName: 'Fan',
                          iconPath: 'assets/images/fan-1.png',
                          powerOn: powerOnFan,
                          onChanged: (value) {
                            setState(() {
                              powerOnFan = value;
                              kirimData(powerOnFan, "fan");
                            });
                          },
                        ),
                      ],
                    ),
                    const SizedBox(height: 10),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      children: [
                        SmartDeviceBox(
                          smartDeviceName: 'Radio',
                          iconPath: 'assets/images/radio.png',
                          powerOn: powerOnRadio,
                          onChanged: (value) {
                            setState(() {
                              powerOnRadio = value;
                              kirimData(powerOnRadio, "radio");
                            });
                          },
                        ),
                        SmartDeviceBox(
                          smartDeviceName: 'TV',
                          iconPath: 'assets/images/tv.png',
                          powerOn: powerOnTV,
                          onChanged: (value) {
                            setState(() {
                              powerOnTV = value;
                              kirimData(powerOnTV, "tv");
                            });
                          },
                        ),
                      ],
                    )

                    // Add your energy page content here
                  ],
                ),
              )
            ],
          ),
        ),
      ),
    );
  }
}
