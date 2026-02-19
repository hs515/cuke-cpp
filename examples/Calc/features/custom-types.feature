# language: en
@ftag
Feature: Custom Types
  Different fruits present different numbers
  As a math idiot
  I want to be told the sum of two fruits

  Scenario Outline: Add two fruits
    Given I have entered an apple into the calculator
    And I have entered a banana into the calculator
    When I press add
    Then the result should be 13 on the screen

  Scenario Outline: Add two fruits
    Given I have entered an apple into the calculator
    And I have entered a cherry into the calculator
    When I press add
    Then the result should be 13 on the screen
