# -*- coding: utf-8 -*-
"""
Created on Fri Oct 15 13:16:43 2021

@author: Nebojsa.Vojinovic
"""
# This allows you to connect to the Workspace you've previously deployed in Azure.
# Be sure to fill in the settings below which can be retrieved by running 'az quantum workspace show' in the terminal.
from azure.quantum import Workspace

# Copy the settings for your workspace below
workspace = Workspace (
  subscription_id = "5e9f578c-16d1-4e61-987f-db672c9bf0cb",
  resource_group = "d5o2",
  name = "d5o2",
  location = "eastus"
)
