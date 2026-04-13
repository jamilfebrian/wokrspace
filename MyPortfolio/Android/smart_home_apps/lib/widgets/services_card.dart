import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class ServicesCard extends StatefulWidget {
  final String title;
  final String icon;
  final Color color;
  final VoidCallback onTap;
  final Color fontColor;

  const ServicesCard({
    required this.title,
    required this.icon,
    required this.color,
    required this.onTap,
    this.fontColor = Colors.white,
    super.key,
  });

  @override
  State<ServicesCard> createState() => _ServicesCardState();
}

class _ServicesCardState extends State<ServicesCard> {
  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: widget.onTap,
      child: Container(
        padding: const EdgeInsets.symmetric(
          vertical: 36,
        ),
        width: 140,
        decoration: BoxDecoration(
          color: widget.color,
          borderRadius: BorderRadius.circular(20),
        ),
        child: Column(
          children: [
            Expanded(child: Image.asset(widget.icon)),
            const SizedBox(height: 10),
            Text(
              widget.title,
              style: TextStyle(
                  fontWeight: FontWeight.bold, color: widget.fontColor),
            )
          ],
        ),
      ),
    );
  }
}

class SmartDeviceBox extends StatelessWidget {
  final String smartDeviceName;
  final String iconPath;
  final bool powerOn;
  final void Function(bool)? onChanged;

  const SmartDeviceBox({
    super.key,
    required this.smartDeviceName,
    required this.iconPath,
    required this.powerOn,
    required this.onChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: const EdgeInsets.all(16),
      width: 150,
      decoration: BoxDecoration(
        color: powerOn ? Colors.indigo : Colors.white,
        borderRadius: BorderRadius.circular(25),
      ),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.spaceAround,
        children: [
          Image.asset(
            iconPath,
            height: 100,
            color: powerOn ? Colors.white : Colors.blue,
          ),
          Row(
            children: [
              Expanded(
                child: Center(
                  child: Text(
                    smartDeviceName,
                    style: TextStyle(
                        fontWeight: FontWeight.bold,
                        fontSize: 16,
                        color: powerOn ? Colors.white : Colors.grey[700]),
                  ),
                ),
              ),
              CupertinoSwitch(
                value: powerOn,
                inactiveTrackColor: Colors.white,
                activeTrackColor: Colors.green.shade500,
                thumbColor: powerOn ? Colors.white : Colors.grey,
                inactiveThumbColor: Colors.grey.shade200,
                onChanged: onChanged,
              )
            ],
          )
        ],
      ),
    );
  }
}
