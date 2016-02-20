//
//  ViewController.swift
//  PeltierControl
//
//  Created by Rohit Banda on 2/19/16.
//  Copyright © 2016 Rohit Banda. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    @IBOutlet weak var peltierTemo: UILabel!
    @IBOutlet weak var outsideTemp: UILabel!
    @IBOutlet weak var tempOverride: UISwitch!
    @IBOutlet weak var overrideTemp: UILabel!
    @IBOutlet weak var uiSliderOv: UISlider!

    
    override func viewDidLoad() {
        super.viewDidLoad()
        tempOverride.setOn(false, animated: true)
        overrideTemp.hidden = true
        uiSliderOv.hidden = true
        peltierTemo.text = " "
        outsideTemp.text = " "
        overrideTemp.text = "\(uiSliderOv.value)"
        
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    @IBAction func manualOvveride(sender: AnyObject) {
        if(tempOverride.on) {
            overrideTemp.hidden = false
            uiSliderOv.hidden = false
        }
        else {
            overrideTemp.hidden = true
            uiSliderOv.hidden = true
        }
    }
    
    @IBAction func uiSliderC(sender: UISlider) {
        overrideTemp.text = "\(Int(uiSliderOv.value))"
    }

}
